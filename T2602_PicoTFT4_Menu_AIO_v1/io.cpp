#include "main.h"
#include "io.h"
#include "atask.h"

typedef struct
{
  uint8_t pin;
  uint32_t pattern;
  uint16_t tick_nbr;
} led_st;

typedef struct
{
  uint8_t pattern_bit;
  uint8_t switches;
  uint8_t tindx;
} io_ctrl_st;

io_ctrl_st io_ctrl;
led_st led[LED_INDX_NBR_OF] = 
{
    [LED_INDX_YELLOW]    = {PIN_LED_YELLOW, 0, 0},
};

const uint32_t led_pattern[BLINK_NBR_OF] = 
{
    0b0000000000000000,
    0b1111111111111111,
    0b1000000000000000,
    0b1000000100000000,
    0b1001001001001000,
    0b1111111100000000,
    0b1111000011110000,
    0b1100110011001100,
    0b1111000000100000,
    0b1111000010001000,
    0b1111001010101010,
    0b1111001100110000,
};


void io_task(void);
//                        123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st io_th      =   {"I/O Task       ", 100,     0,     0,  255,    0,  1,  io_task };


void io_initialize(void)
{
    // pinMode(PIN_TFT_LED, OUTPUT);
    pinMode(PIN_PIR, INPUT);
    pinMode(PIN_LDR_AN, INPUT);
    pinMode(PIN_ABTN,INPUT);
    analogReadResolution(12);
    analogWrite(PIN_TFT_LED,200);
    io_ctrl.pattern_bit = 0;
    for (uint8_t lindx = 0; lindx < LED_INDX_NBR_OF; lindx++ )
    {
       pinMode(led[lindx].pin, OUTPUT);
    }   
} 

void io_task_initialize(void)
{
   io_ctrl.tindx =  atask_add_new(&io_th);
}

uint16_t io_read_ldr(void)
{
  return analogRead(PIN_LDR_AN);
}


uint16_t io_read_akbd(void)
{
  return analogRead(PIN_ABTN);
}

void io_led_flash(led_index_et lindx, blink_et bindx, uint16_t tick_nbr)
{
  led[lindx].pattern = led_pattern[bindx];
  led[lindx].tick_nbr = tick_nbr;
}

void io_task(void)
{

    uint32_t patt = 1UL << io_ctrl.pattern_bit;
    for (uint8_t i = 0; i <= LED_INDX_NBR_OF; i++)
    {
        if (led[i].tick_nbr > 0) {
            if ((led[i].pattern & patt) != 0)
                digitalWrite(led[i].pin, HIGH);
            else  
                digitalWrite(led[i].pin, LOW);
            led[i].tick_nbr--;  
        }
    } 
    if (++io_ctrl.pattern_bit >= 16) io_ctrl.pattern_bit = 0;
}