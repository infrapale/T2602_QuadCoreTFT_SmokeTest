#ifndef __SENSOR_H__
#define __SENSOR_H__

#define SENSOR_NBR_OF 4
#define SENSOR_MAX_AGE  (3600*24*365)


typedef struct 
{
    char        tag[CMD_FIELD_LEN];
    char        zone[CMD_FIELD_LEN];
    char        sensor[CMD_FIELD_LEN];
    char        value[CMD_FIELD_LEN];
    char        remark[CMD_FIELD_LEN];
    uint32_t    age;
    int16_t     io_indx;
} sensor_field_st;

void sensor_initialize(void);

void sensor_task(void);

void sensor_update(msg_data_st *msg);

#endif