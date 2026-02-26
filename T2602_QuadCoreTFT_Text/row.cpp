#include "main.h"
#include "box.h"

#define ROW_NBR_OF  10
#define ROW_LEN     80

typedef struct 
{
    uint8_t cursor;
} row_ctrl_st;

typedef struct
{
    char text[ROW_LEN];
    uint16_t fill_color;
    uint16_t border_color;
    uint16_t text_color;
    // uint8_t  font_indx;
    // uint8_t  font_size;
} row_st;

row_st row[ROW_NBR_OF] = {0};
row_ctrl_st row_ctrl = 
{
    .cursor = 0
};

void row_initialize(void)
{
    for(uint8_t i= 0; i < ROW_NBR_OF; i++)
    {
        row[i].border_color = TFT_BLACK;
        row[i].fill_color = TFT_BLACK;
        row[i].text_color = TFT_YELLOW;
    }
}

void row_print(char *txtp)
{
    for (uint8_t r = ROW_NBR_OF -1; r >= 1; r-- )
    {
        memcpy(&row[r],&row[r-1], sizeof(row_st));
    }
    strncpy(row[0].text, txtp, ROW_LEN);
    for (uint8_t r = 0; r < ROW_NBR_OF; r++ ) {
        box_print(r, row[r].text);
    }
}



