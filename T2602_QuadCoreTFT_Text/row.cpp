#include "main.h"
#include "box.h"

#define ROW_NBR_OF  10
#define ROW_LEN     40

typedef struct 
{
    uint8_t cursor;
} row_ctrl_st;

char row[ROW_NBR_OF][ROW_LEN] = {0};
row_ctrl_st row_ctrl = 
{
    .cursor = 0
}





