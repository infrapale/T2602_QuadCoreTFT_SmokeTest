#include "main.h"
#include "uart.h"
#include "io.h"

void uart_read_rfm_uart(void)
{
    if (SerialRFM.available())
    {
        String Str = SerialRFM.readStringUntil('\n');
        if (Str.length()> 0)
        {
            Serial.println(Str);
        }
    } 

// }