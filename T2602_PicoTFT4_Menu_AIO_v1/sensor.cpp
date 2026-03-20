#include <stdio.h>
#include <string.h>
#include "main.h"
#include "comm.h"
#include "atask.h"
#include "sensor.h"

#define SENSOR_BLE_NBR_OF   2
sensor_field_st sensor[SENSOR_NBR_OF] =
{
    {"R1X1J1:" ,"Dock", "T_bmp1",   "0.0", "", 0, 1},
    {"R1X1J1:" ,"Dock", "T_dht22",  "0.0", "", 0, 2},
    {"R1X1J1:" ,"Dock", "T_Water",  "0.0", "", 0, 3},
    {"R1X1J1:" ,"Dock", "ldr1",     "0.0", "", 0, -1},
};

sensor_ruuvi_st ruuvi[SENSOR_BLE_NBR_OF] =
{
    {.name="MH1", .temp=0.0, .hum=0.0, .batt=0.0, .updated=false},
    {.name="Parvi", .temp=0.0, .hum=0.0, .batt=0.0, .updated=false},
};

// Function Prototypes
void sensor_task(void);

//                      123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st sensor_h  =   {"Sensor SM      ", 1000,    0,     0,  255,    0,   1,  sensor_task };

// extern comm_st comm[];

void sensor_initialize(void)
{
    atask_add_new(&sensor_h);
}

void sensor_task(void)
{
    for(uint8_t i = 0; i < SENSOR_NBR_OF; i++)
    {
        if((sensor[i].age > 0) && (sensor[i].age < SENSOR_MAX_AGE)) sensor[i].age++; 
    }
}

void sensor_ruuvi_update(comm_data_st *msg)
{
    // <##B1T1=;Parvi;23.5;45;2.80>
    // <##B1T1=;MH1;24.5;34;2.91>
    uint8_t sindx = 255;
    Serial.printf("Sensor update: %s, %s, %s, %s, %s\n",
        msg->field[0], msg->field[1], msg->field[2], msg->field[3], msg->field[4]);
    
    for(uint8_t i= 0; (i < SENSOR_BLE_NBR_OF) && (sindx==255); i++)
    {
        if( strncmp(msg->field[1],ruuvi[i].name,CMD_FIELD_LEN) == 0)
        {
            sindx = i;
            ruuvi[i].temp = strtof(msg->field[2], NULL);
            ruuvi[i].hum = strtof(msg->field[3], NULL);
            ruuvi[i].batt = strtof(msg->field[4], NULL);
            ruuvi[i].updated = true;
        }
    }
}

void sensor_debug(void)
{
    for(uint8_t i= 0; (i < SENSOR_BLE_NBR_OF); i++)
    {
        Serial.printf("Ruuvi: %s: %0.2f %0.2f %0.2f %d\n",
            ruuvi[i].name,
            ruuvi[i].temp,
            ruuvi[i].hum,
            ruuvi[i].batt,
            ruuvi[i].updated );
    }
}
