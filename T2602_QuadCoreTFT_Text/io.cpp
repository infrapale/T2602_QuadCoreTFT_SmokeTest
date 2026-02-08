#include "io.h"


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
