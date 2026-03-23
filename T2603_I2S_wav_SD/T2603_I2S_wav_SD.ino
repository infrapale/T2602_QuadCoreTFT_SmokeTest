#include <I2S.h>        // Arduino-Pico I2S library
#include <SPI.h>
#include "SdFat.h"
//#include <SD.h>

#include "main.h"
#include "io.h"

#define SD_CS 13
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

typedef struct
{
    bool enable;
    FsFile file_1;

} sd_ctrl_st;

sd_ctrl_st sd_ctrl = {.enable=false};

SdFat SD;
WAVHeader header;
SdSpiConfig config(PIN_SD_CS , DEDICATED_SPI, SD_SCK_MHZ(16), &SPI1);
I2S i2s(OUTPUT, PIN_I2S_BCLK, PIN_I2S_LRCLK, PIN_I2S_DOUT);

void sd_initialize(void)
{
    SPI1.setRX(PIN_SPI_1_MISO); // or setMISO()
    SPI1.setCS(PIN_SD_CS);
    SPI1.setSCK(PIN_SPI_1_SCK);
    SPI1.setTX(PIN_SPI_1_MOSI); // or setMOSI()
    SPI1.begin(false);

    uint8_t retry_cntr = 0;

    while ( (retry_cntr < SD_RETRY_MAX ) && !sd_ctrl.enable)
    {
        sd_ctrl.enable = SD.begin(config);
        retry_cntr++;
        delay(1000); // Wait for a second before retrying        
    }
    if(sd_ctrl.enable) Serial.println("initialization done.");
    else Serial.println("initialization failed!");
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  sd_initialize();
  // Serial.println("Initializing SD...");
  // if (!SD.begin(SD_CS)) {
  //   Serial.println("SD init failed!");
  //   while (1);
  // }
  // Serial.println("SD OK");

  wavFile = SD.open("dial_call.wav");
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
  // i2s.setBCLK(PIN_I2S_BCLK);     // BCLK pin
  // i2s.setDATA(PIN_I2S_DOUT);      // DIN pin
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
  Serial.println(bytesRead);
  // Write raw PCM data to I2S
  for (int i = 0; i < bytesRead; i += (header.bitsPerSample / 8)) {
    int32_t sample = 0;

    if (header.bitsPerSample == 16) {
      sample = (int16_t)(buffer[i] | (buffer[i+1] << 8));
      sample <<= 16; // left-align for 32-bit I2S
    }

    i2s.write(sample);
  }
}