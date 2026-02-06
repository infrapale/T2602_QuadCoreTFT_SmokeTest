#include "io.h"

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
#define PIN_SPI1_SCK    (10u)
#define PIN_SPI1_MOSI   (11u)
#define PIN_SPI1_MISO   (12u)
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

void io_initialize(void)
{
    pinMode(PIN_TFT_RESET, OUTPUT);
    pinMode(PIN_SD_CS, OUTPUT);
    pinMode(PIN_TFT_BL, OUTPUT);
    pinMode(PIN_KBD_LED, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_RUN_RFM, OUTPUT);
    pinMode(PIN_LDR_AN, INPUT);
    pinMode(PIN_LDR_AN, INPUT);

    analogReadResolution(12);

}

void io_task(void)
{

}
