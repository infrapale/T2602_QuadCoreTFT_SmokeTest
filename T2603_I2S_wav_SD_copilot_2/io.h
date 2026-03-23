#ifndef __IO_H__ 
#define __IO_H__
#include "Arduino.h"
#include "main.h"
//#define BOARD_PICO_TFT_4KEYS

#define I2C_ADDR_DS3231     0x68 
#define I2C_ADDR_AT24C32    0x57 
 


// TFT PIN Definitions
#if TFT_TARGET_BOARD == BOARD_PICO_TFT_4KEYS
#define PIN_WIRE_SDA          (12u)
#define PIN_WIRE_SCL          (13u)

#define PIN_KEY1		      (28u)
#define PIN_KEY2		      (27u)
#define PIN_KEY3		      (17u)
#define PIN_KEY_STATUS        (2u)
#define PIN_LDR_ANALOG_INP    (26u)
#define PIN_PIR_INP           (5u)
// TFT
#define PIN_TFT_LED           (10u)
#define PIN_TFT_CS            (9u)
#define PIN_TFT_RST           (14u)
#define PIN_TFT_DC            (15u)
#define PIN_TFT_MISO          (16u)
#define PIN_TFT_CLK           (18u)
#define PIN_TFT_MOSI          (19u)
#define PIN_TOUCH_CS          (21u)

#elif TFT_TARGET_BOARD == BOARD_TFT_4_QUADCORE_PICO
#define PIN_TX0         (0u)
#define PIN_RX0         (1u)
#define PIN_I2C1_SDA    (2u)
#define PIN_I2C1_SCL    (3u)
#define PIN_TX1         (4u)
#define PIN_RX1         (5u)
// I2S Audio Out
#define PIN_I2S_BCLK    (6u)
#define PIN_I2S_LRCLK   (7u)
#define PIN_I2S_DOUT    (8u)
// SD Card SPI
#define PIN_SPI_1_SCK   (10u)
#define PIN_SPI_1_MOSI  (11u)
#define PIN_SPI_1_MISO  (12u)
#define PIN_SD_CS       (13u)
// TFT SPI
#define PIN_TFT_RST     (9u)
#define PIN_TFT_LED     (14u)
#define PIN_TFT_CS      (17u)
#define PIN_TFT_DC      (15u)
#define PIN_TFT_MISO    (16u)
#define PIN_TFT_CLK     (18u)
#define PIN_TFT_MOSI    (19u)
#define PIN_TOUCH_CS    (-1)
// Mixed
// #define PIN_KBD_LED     (20u)
#define PIN_PIR         (20u)
#define PIN_LED_YELLOW  (21u)
#define PIN_RUN_RFM     (22u)
#define PIN_LDR_AN      (26u)
#define PIN_ABTN        (27u)


typedef enum
{
    LED_INDX_YELLOW =0,
    //LED_INDX_RED,
    //LED_INDX_BLUE,
    LED_INDX_NBR_OF
} led_index_et;
#endif

typedef enum
{
  BLINK_OFF = 0,
  BLINK_ON,
  BLINK_SHORT_FLASH,
  BLINK_FLASH,
  BLINK_FAST_FLASH,
  BLINK_SLOW_BLINK,
  BLINK_BLINK,
  BLINK_FAST_BLINK,
  BLINK_CLIENT,
  BLINK_RELIABLE_CLIENT,
  BLINK_SERVER,
  BLINK_RELIABLE_SERVER,
  BLINK_NBR_OF
} blink_et;

// MISO 16 MOSI 19 SCLK 18 CS 13 DC 15 RST 9 BL 13 T_CS 21
//#define PIN_TFT_CS            (17u)

// TFT Library Check
typedef struct 
{
    char        label[5];
    int8_t      design_pin;
    int8_t      library_pin;
} tft_pin_check_st;

void io_initialize(void);
void io_task_initialize(void);

uint16_t io_read_ldr(void);
uint16_t io_read_akbd(void);
void    io_led_flash(led_index_et lindx, blink_et bindx, uint16_t tick_nbr);
void    io_task(void);



#endif
