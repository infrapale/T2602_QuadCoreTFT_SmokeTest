#include "main.h"
#include "io.h"


void io_initialize(void)
{
    pinMode(PIN_TFT_LED, OUTPUT);
    // pinMode(PIN_PIR_INP,INPUT);
    pinMode(PIN_LDR_AN,INPUT);
    pinMode(PIN_ABTN,INPUT);
    analogReadResolution(12);
    analogWrite(PIN_TFT_LED,200);
}


uint16_t io_read_ldr(void)
{
  return analogRead(PIN_LDR_AN);
}


uint16_t io_read_akbd(void)
{
  return analogRead(PIN_ABTN);
}