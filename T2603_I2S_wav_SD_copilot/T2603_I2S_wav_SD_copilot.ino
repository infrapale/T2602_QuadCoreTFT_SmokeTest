#include <I2S.h>
#include <SPI.h>
#include "SdFat.h"

#include "main.h"
#include "io.h"

#define SD_RETRY_MAX 8

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

// *** FIX 1: Use correct 3‑pin I2S constructor ***
I2S i2s(OUTPUT, PIN_I2S_BCLK, PIN_I2S_LRCLK, PIN_I2S_DOUT);

// *** FIX 3: Lower SPI speed to avoid I2S starvation ***
SdSpiConfig config(PIN_SD_CS, DEDICATED_SPI, SD_SCK_MHZ(8), &SPI1);

void sd_initialize(void)
{
    SPI1.setRX(PIN_SPI_1_MISO);
    SPI1.setCS(PIN_SD_CS);
    SPI1.setSCK(PIN_SPI_1_SCK);
    SPI1.setTX(PIN_SPI_1_MOSI);
    SPI1.begin(false);

    uint8_t retry_cntr = 0;

    // while ((retry_cntr < SD_RETRY_MAX) && !SD.cardBegin(config)) {
    while ((retry_cntr < SD_RETRY_MAX) && !SD.begin(config)) {
        retry_cntr++;
        delay(500);
    }

    if (retry_cntr < SD_RETRY_MAX)
        Serial.println("SD initialization done.");
    else
        Serial.println("SD initialization failed!");
}

void setup() {
  Serial.begin(9600);
  delay(2500);

  sd_initialize();

  wavFile = SD.open("bigben.wav");
  if (!wavFile) {
    Serial.println("Failed to open WAV file");
    while (1);
  }

  // Read WAV header
  wavFile.read(&header, sizeof(WAVHeader));

  Serial.print("Sample rate: ");
  Serial.println(header.sampleRate);
  Serial.print("Bits: ");
  Serial.println(header.bitsPerSample);
  Serial.print("Channels: ");
  Serial.println(header.numChannels);

  // Configure I2S
  i2s.setBitsPerSample(header.bitsPerSample);
  i2s.setFrequency(header.sampleRate);

  if (!i2s.begin()) {
    Serial.println("I2S begin failed!");
    while (1);
  }

  Serial.println("Playing...");
}

void loop() {
  static uint8_t buffer[512];

  if (!wavFile.available()) {
    Serial.println("Done.");
    wavFile.close();
    while (1);
  }

  int bytesRead = wavFile.read(buffer, sizeof(buffer));
  if (bytesRead <= 0) return;
  //Serial.println(bytesRead);
  int bytesPerSample = header.bitsPerSample / 8;
  int frameSize = bytesPerSample * header.numChannels;

  // *** FIX 2: Proper stereo frame stepping ***
  for (int i = 0; i + frameSize <= bytesRead; i += frameSize) {

    int16_t sample16 = 0;

    if (header.numChannels == 1) {
      // Mono
      sample16 = (int16_t)(buffer[i] | (buffer[i+1] << 8));
    } else {
      // Stereo → mono downmix
      int16_t left  = (int16_t)(buffer[i] | (buffer[i+1] << 8));
      int16_t right = (int16_t)(buffer[i+2] | (buffer[i+3] << 8));
      sample16 = (left / 2) + (right / 2);
    }

    // Convert 16‑bit PCM to 32‑bit left‑aligned I2S
    int32_t sample32 = ((int32_t)sample16) << 16;

    i2s.write(sample32);
    // i2s.write(sample32);
  }
}