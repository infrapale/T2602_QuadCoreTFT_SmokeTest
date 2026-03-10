/************************************************************************************************************************************
@title T2602_PicoTFT28_AIO
@git  	https://github.com/infrapale/T2602_PicoTFT28_AIO.git
************************************************************************************************************************************
@HW Raspberry Pi Pico W on a pico TFT classic board (2.8" TFT display + 4 Cherry key buttons)

************************************************************************************************************************************
https://github.com/adafruit/Adafruit_MQTT_Library
https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial-using-semaphore-and-mutex-in-freertos-with-arduino
https://learn.adafruit.com/dvi-io/code-the-dashboard
***********************************************************************************************************************************/


#include "main.h"
#include <SPI.h>
#include "io.h"
#include "menu.h"
#include "time_func.h"
#include "atask.h"
#include "aio_mqtt.h"
#include "dashboard.h"
#include "io.h"
#include "comm.h"
#include "sensor.h"
#include "box.h"
#include "eeprom.h"


void print_debug_task(void)
{
  //atask_print_status(true);
  //aio_mqtt_debug_print();
  //dashboard_debug_print();
  Serial.printf("LDR  = %d\n", io_read_ldr());
  Serial.printf("AKBD = %d\n", io_read_akbd());
}

//                                  123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st debug_task_handle    =   {"Debug Task     ", 2000,    0,     0,  255,    0,  1,  print_debug_task };



uint32_t  targetTime = 0; 

void run_tft_pin_check(void);

void setup(void) {
    delay(2000);
    Serial.begin(115200); // For debug
    delay(2000);
    while (!Serial);
    Serial.println(F(APP_NAME));
    Serial.printf(" Compiled: %s %s\n",__DATE__, __TIME__);
    Wire1.setSDA(PIN_I2C1_SDA);
    Wire1.setSCL(PIN_I2C1_SCL);
    Wire1.begin();
    time_begin();
    atask_initialize();
    io_initialize();
    eeprom_initialize();
    atask_add_new(&debug_task_handle);
    comm_initialize();
    sensor_initialize();
    
    dashboard_initialize();   // start dashboard task
    menu_initialize();        // starting scan and read tasks
    aio_mqtt_initialize();    // 


    box_run_tft_pin_check();
    box_initialize();
    box_structure_print();
}

void setup1()
{
  //Watchdog.reset();
  targetTime = millis() + 100;
}

// Fast running loop
void loop() 
{
  atask_run();
}

// Slow running loop: WiFI an MQTT
void loop1()
{
  if (millis() > targetTime)
  {
    aio_mqtt_stm();
    targetTime = millis() + 100;
  }
}
