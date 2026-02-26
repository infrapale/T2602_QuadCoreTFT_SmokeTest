#include "main.h"
#include "uart.h"
#include "io.h"
#include "row.h"

void uart_read_rfm_uart(void)
{
    if (SerialRFM.available())
    {
        String Str = SerialRFM.readStringUntil('\n');
        if (Str.length()> 0)
        {
            char buff[80];
            Str.toCharArray(buff, 80);
            Serial.println(Str);
            row_print(buff);
        }
    } 

}