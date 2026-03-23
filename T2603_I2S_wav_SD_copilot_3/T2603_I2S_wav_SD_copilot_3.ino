#include <I2S.h>
#include <SPI.h>
#include "SdFat.h"

#include "main.h"
#include "io.h"

#define SD_RETRY_MAX 8
#define AUDIO_BUFFER_SIZE 512   // bytes per buffer
#define VOLUME_DEFAULT 0.5f     // 0.0 .. 1.0

FsFile wavFile;

// WAV header structure
struct WAVHeader {
  char riff[4];
  uint32_t chunkSize;
  char wave[4];
  char fmt[4];
  uint32_t subchunk1Size;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;
  char dataHeader[4];
  uint32_t dataSize;
};

SdFat SD;
WAVHeader header;

// I2S: BCLK, LRCLK, DOUT
I2S i2s(OUTPUT, PIN_I2S_BCLK, PIN_I2S_LRCLK, PIN_I2S_DOUT);

// Safer SPI speed
SdSpiConfig config(PIN_SD_CS, DEDICATED_SPI, SD_SCK_MHZ(12), &SPI1);

// Double buffers
uint8_t audioBuf[2][AUDIO_BUFFER_SIZE];
volatile int activeBuf = 0;
volatile int filled[2] = {0, 0};

float volume = VOLUME_DEFAULT;

void sd_initialize(void)
{
    SPI1.setRX(PIN_SPI_1_MISO);
    SPI1.setCS(PIN_SD_CS);
    SPI1.setSCK(PIN_SPI_1_SCK);
    SPI1.setTX(PIN_SPI_1_MOSI);
    SPI1.begin(false);

    uint8_t retry_cntr = 0;

    while ((retry_cntr < SD_RETRY_MAX) && !SD.begin(config)) {
        retry_cntr++;
        delay(500);
    }

    if (retry_cntr < SD_RETRY_MAX)
        Serial.println("SD initialization done.");
    else {
        Serial.println("SD initialization failed!");
        while (1);
    }
}

bool read_wav_header()
{
    if (wavFile.read(&header, sizeof(WAVHeader)) != sizeof(WAVHeader)) {
        Serial.println("Header read failed");
        return false;
    }

    Serial.print("Sample rate: ");   Serial.println(header.sampleRate);
    Serial.print("Bits: ");          Serial.println(header.bitsPerSample);
    Serial.print("Channels: ");      Serial.println(header.numChannels);
    Serial.print("audioFormat: ");   Serial.println(header.audioFormat);
    Serial.print("dataHeader: ");    Serial.write(header.dataHeader, 4); Serial.println();
    Serial.print("dataSize: ");      Serial.println(header.dataSize);

    if (strncmp(header.riff, "RIFF", 4) != 0) return false;
    if (strncmp(header.wave, "WAVE", 4) != 0) return false;
    if (strncmp(header.fmt,  "fmt ", 4) != 0) return false;
    if (strncmp(header.dataHeader, "data", 4) != 0) return false;
    if (header.audioFormat != 1) return false;        // PCM
    if (header.bitsPerSample != 16) return false;     // 16‑bit
    if (header.numChannels != 1) return false;        // mono
    if (header.sampleRate != 16000) {
        Serial.println("WAV must be 16 kHz mono 16‑bit PCM");
        return false;
    }

    return true;
}

void setup() {
  Serial.begin(115200);
  delay(2500);

  sd_initialize();

  wavFile = SD.open("bigben-16.wav");
  if (!wavFile) {
    Serial.println("Failed to open WAV file");
    while (1);
  }

  if (!read_wav_header()) {
    Serial.println("Invalid WAV file");
    while (1);
  }

  // I2S: fixed 16 kHz mono, 16‑bit
  i2s.setStereo(true);
  i2s.setBitsPerSample(16);
  i2s.setFrequency(16000);

  if (!i2s.begin()) {
    Serial.println("I2S begin failed!");
    while (1);
  }

  // Prime both buffers
  for (int b = 0; b < 2; ++b) {
    int n = wavFile.read(audioBuf[b], AUDIO_BUFFER_SIZE);
    filled[b] = n;
  }

  activeBuf = 0;

  Serial.println("Playing (double‑buffered, 16 kHz mono)...");
}

void loop() {
  int buf = activeBuf;

  // If current buffer empty, swap and refill the other
  if (filled[buf] <= 0) {
    int next = 1 - buf;

    // Refill next buffer if file still has data
    if (wavFile.available()) {
      int n = wavFile.read(audioBuf[next], AUDIO_BUFFER_SIZE);
      filled[next] = n;
    } else {
      filled[next] = 0;
    }

    activeBuf = next;
    buf = next;

    // If both empty -> done
    if (filled[buf] <= 0) {
      Serial.println("Done.");
      wavFile.close();
      while (1);
    }
  }

  // Play from current buffer
  int bytesRead = filled[buf];
  int16_t *samples = (int16_t *)audioBuf[buf];
  int sampleCount = bytesRead / 2; // 2 bytes per sample

  for (int i = 0; i < sampleCount; ++i) {
    int16_t s = samples[i];

    // Volume control
    int32_t scaled = (int32_t)(s * volume);
    if (scaled > 32767) scaled = 32767;
    if (scaled < -32768) scaled = -32768;

    int32_t sample32 = (scaled << 16);

    // Mono: send same sample to L and R
    while (i2s.availableForWrite() < 2) {
      // wait for space (keeps FIFO from underrun)
      tight_loop_contents();
    }

    i2s.write(sample32); // Left
    i2s.write(sample32); // Right
  }

  filled[buf] = 0; // mark buffer as consumed
}