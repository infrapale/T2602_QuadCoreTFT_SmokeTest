#ifndef __IO_H__
#define __IO_H__
#include "Arduino.h"

#define SerialRFM       Serial1

#define PIN_TX0         (0u)
#define PIN_RX0         (1u)
#define PIN_I2C1_SDA    (2u)
#define PIN_I2C1_SCL    (3u)
#define PIN_TX1         (4u)
#define PIN_RX1         (5u)
#define PIN_I2S_BCLK    (6u)
#define PIN_I2S_LRCK    (7u)
#define PIN_I2S_DOUT    (8u)
#define PIN_TFT_RESET   (9u)

#define PIN_SD_SCK      (10u)
#define PIN_SD_MOSI     (11u)
#define PIN_SD_MISO     (12u)
#define PIN_SD_CS       (13u)

#define PIN_TFT_BL      (14u)
#define PIN_TFT_DC      (15u)
#define PIN_SPI0_MISO   (16u)
#define PIN_TFT_CS      (17u)
#define PIN_SPI0_SCK    (18u)
#define PIN_SPI0_MOSI   (19u)
#define PIN_KBD_LED     (20u)
#define PIN_LED_YELLOW  (21u)
#define PIN_RUN_RFM     (22u)
#define PIN_LDR_AN      (26u)
#define PIN_ABTN        (27u)

void io_initialize(void);

void io_task(void);

#endif