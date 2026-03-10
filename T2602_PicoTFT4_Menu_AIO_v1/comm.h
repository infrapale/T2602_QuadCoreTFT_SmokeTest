#ifndef __COMM_H__
#define __COMM_H__

#define CMD_TAG_LEN     5
#define CMD_MAX_VALUES  8
#define CMD_FIELD_LEN   8
#define COMM_BUFF_LEN   64

typedef enum
{
    COMM_INDX_0   = 0,
    COMM_INDX_1,
    COMM_INDX_2,
    COMM_INDX_NBR_OF,
} comm_indx_et;

typedef struct
{
    char tx_buff[COMM_BUFF_LEN];
    char rx_buff[COMM_BUFF_LEN];
    bool rx_avail;
} comm_st;

typedef struct
{
  char      tag[CMD_TAG_LEN];
  //cmd_et    tag_indx;
  uint8_t   nbr_values;
  int16_t   value[CMD_MAX_VALUES];
  char      field[CMD_MAX_VALUES][CMD_FIELD_LEN];
  uint8_t   target;
  uint8_t   sender;
} msg_data_st;

typedef struct
{
    uint32_t send_time_upate_at;
    msg_data_st msg;
} comm_ctrl_st;



void comm_initialize(void);

void comm_1_read(void);

#endif