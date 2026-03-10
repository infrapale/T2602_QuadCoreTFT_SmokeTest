#include "main.h"
#include "comm.h"
#include "atask.h"
#include "sensor.h"

sensor_field_st sensor[SENSOR_NBR_OF] =
{
    {"R1X1J1:" ,"Dock", "T_bmp1",   "0.0", "", 0, 1},
    {"R1X1J1:" ,"Dock", "T_dht22",  "0.0", "", 0, 2},
    {"R1X1J1:" ,"Dock", "T_Water",  "0.0", "", 0, 3},
    {"R1X1J1:" ,"Dock", "ldr1",     "0.0", "", 0, -1},
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

void sensor_update(msg_data_st *msg)
{
    Serial.printf("Sensor update: %s, %s, %s, %s, %s\n",
        msg->field[0], msg->field[1], msg->field[2], msg->field[3], msg->field[4]);
}