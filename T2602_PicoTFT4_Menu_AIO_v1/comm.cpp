#include "main.h"
#include "comm.h"
#include "atask.h"
#include "time_func.h"
#include "sensor.h"


void comm_1_task(void);
bool comm_parse(char *msg);

//                      123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st comm1_h  =   {"Comm 1 SM      ", 100,    0,     0,  255,    0,   1,  comm_1_task };


comm_st comm[COMM_INDX_NBR_OF];
comm_ctrl_st comm_ctrl = {0};

void comm_initialize(void)
{
  Serial1.setTX(PIN_TX0);   
  Serial1.setRX(PIN_RX0);
  Serial2.setTX(PIN_TX1);   
  Serial2.setRX(PIN_RX1);
  Serial1.begin(9600);
  // Serial1.begin(9600);

  atask_add_new(&comm1_h);

}

void comm_1_read(void)
{
    if (Serial1.available())
    {
        String RxStr;
        Serial.println("rx0 is available");
        RxStr = Serial1.readStringUntil('\n');
        if (RxStr.length()> 0)
        {
            comm[COMM_INDX_1].rx_avail = true;
            RxStr.toCharArray(comm[COMM_INDX_1].rx_buff, COMM_BUFF_LEN);
            Serial.println(comm[COMM_INDX_1].rx_buff);
        }
    } 
}

void comm_1_task(void)
{
    bool is_ok;
    switch(comm1_h.state)
    {
      case 0:
        comm[COMM_INDX_1].rx_avail = false;
        comm1_h.state = 10;
        comm_ctrl.send_time_upate_at = millis() + 60000;
        break;
      case 10:
        comm_1_read();
        if(comm[COMM_INDX_1].rx_avail) comm1_h.state = 100;
        else comm1_h.state = 50;
        break;
      case 50:
        if(millis() > comm_ctrl.send_time_upate_at){
          comm_ctrl.send_time_upate_at = millis() + 60000;
          comm1_h.state = 60;
        } 
        else comm1_h.state = 10;
        break;
      case 60:
        time_to_encode(comm[COMM_INDX_1].tx_buff);
        Serial.printf("* * * transmit time %s * * * \n", comm[COMM_INDX_1].tx_buff);
        comm1_h.state = 10;
        break;  
      case 100:
        Serial.printf("Parse comm 1: %s\n",comm[COMM_INDX_1].rx_buff);
        is_ok = comm_parse(comm[COMM_INDX_1].rx_buff);
        if(is_ok) sensor_update(&comm_ctrl.msg);

        comm[COMM_INDX_1].rx_avail = false;
        comm1_h.state = 10;
        break;
      case 110:
        comm1_h.state = 10;
        break;
    }
}

bool comm_parse(char *msg)
{
    String  Str = msg;
    String  SubStr;
    int     indx1 = Str.indexOf('<');
    int     indx2 = Str.indexOf('>');
    bool    do_continue = true;
    bool    do_iterate = true;
    int     end_pos = indx2;
    uint8_t field_indx;

    if ((indx1 >= 0)&&(indx2>indx1)) do_continue = true;
    else do_continue = false;
    if (do_continue) {

        SubStr = Str.substring(indx1,indx2);
        //Serial.print("SubStr="); Serial.println(SubStr);
        end_pos = SubStr.length();
        field_indx = 0;
        if((indx1 < end_pos)) do_continue = true;
        else do_continue = false;
    }
    //memset(msg_data->field,0x00, sizeof(msg_data->field));
    for (uint8_t i=0; i < CMD_MAX_VALUES; i++) comm_ctrl.msg.field[i][0] = 0x00;
    indx1++;

    do_iterate = do_continue;   
    while(do_iterate)
    {
        indx2 = SubStr.indexOf(';',indx1);
        if (indx2 < 0) {
            indx2 = end_pos;
            do_iterate = false;
        }
        //msg_data->value[msg_data->nbr_values] = StrP->substring(indx1,indx2).toInt();
        SubStr.substring(indx1,indx2).toCharArray(comm_ctrl.msg.field[field_indx],CMD_FIELD_LEN);
        indx1 = indx2+1;
        field_indx++;
        if ((indx2 >= end_pos) || (field_indx >= CMD_MAX_VALUES)) do_iterate = false;
    }

    Serial.print("** Fields: ");
    if(!do_continue) Serial.print(" !incorrect: "); 
    comm_ctrl.msg.nbr_values = field_indx;
    for (uint8_t i=0; i < comm_ctrl.msg.nbr_values; i++)
    {
        Serial.printf("%s;", comm_ctrl.msg.field[i] );
    }
    Serial.println();
    return do_continue;
}