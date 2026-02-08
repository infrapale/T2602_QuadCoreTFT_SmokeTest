#include "main.h"
#include "box.h"

// #define   TFT_WIDTH   480
// #define   TFT_HEIGHT  320

/*
    Heightx1                               Heightx2               Heightx4
    ------------------------------------   -------------------   -------------------
    |                                  |   |                 |   |                 |
    |..................................|   |                 |   |                 |
    |                                  |   |                 |   |                 |
    |..................................|   -------------------   |                 |
    |                                  |   |                 |   |                 |
    |..................................|   |                 |   |                 |
    |                                  |   |                 |   |                 |
    |..................................|   -------------------   -------------------
    |                                  |   |                 |   |                 |
    |..................................|   |                 |   |                 |
    |                                  |   |                 |   |                 |
    |..................................|   -------------------   |                 |
    |                                  |   |                 |   |                 |
    |..................................|   |                 |   |                 |
    |                                  |   |                 |   |                 |  
    ------------------------------------   -------------------  -------------------





*/

#include "main.h"
#include "box.h"
#include "io.h"

//#include "aio_mqtt.h"
//#include "time_func.h"
//#include "menu.h"
//#include "atask.h"

#define BOX_NBR_OF           12
#define BOX_TEXT_LEN        40

typedef struct
{
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t width;
    uint16_t height;
    char     txt[BOX_TEXT_LEN];
    uint8_t  font_indx;
    uint8_t  font_size;
    uint16_t fill_color;
    uint16_t border_color;
    uint16_t text_color;
} box_st;

extern TFT_eSPI tft;

box_st box[BOX_NBR_OF] =
{
  // x    y    w    h   label  fon f  fill color  border color  text color
  {  0,   0, 480, 320, "12345", 8, 1, TFT_BLACK, TFT_LIGHTGREY, TFT_LIGHTGREY },
  {  0,   0, 480,  32, "Box 1", 4, 1, TFT_BLACK, TFT_GOLD, TFT_GOLD },
  {  0,  32, 480,  32, "Box 2", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE },
  {  0,  64, 480,  32, "Box 3", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE },
  {  0,  96, 480,  32, "Box 4", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0, 128, 480,  32, "Box 5", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0, 160, 480,  32, "Box 6", 4, 1, TFT_BLACK, TFT_VIOLET, TFT_GOLD },
  {  0, 192, 480,  32, "Box 7", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0, 224, 480,  32, "Box 8", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0, 256, 480,  32, "Box 9", 2, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0, 288, 480,  32, "Box 10", 1, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0, 128, 480,  96, "Box 11", 8, 1, TFT_YELLOW, TFT_BLUE, TFT_BLACK},

 
};


void box_clear(void)
{
    tft.setRotation(1);
    tft.setTextSize(1);
    tft.fillScreen(TFT_BLUE);
    //box_draw(0);  // clear dashboard
}

void box_initialize(void)
{
    //atask_add_new(&dashboard_task_handle);
    pinMode(PIN_TFT_BL, OUTPUT);
    digitalWrite(PIN_TFT_BL,HIGH);
    // pinMode(PIN_PIR_INP,INPUT);
    // pinMode(PIN_LDR_ANALOG_INP,INPUT);
    // analogReadResolution(12);
    // analogWrite(PIN_TFT_LED_OUT,30);
    tft.init();
    box_clear();
}


void box_draw(uint8_t bindx)
{
    tft.setTextSize(box[bindx].font_size);
    tft.setTextColor(box[bindx].text_color, box[bindx].fill_color, false);
    tft.fillRect(box[bindx].x_pos, box[bindx].y_pos, box[bindx].width, box[bindx].height, box[bindx].fill_color);
    tft.drawString(box[bindx].txt , box[bindx].x_pos+4, box[bindx].y_pos+2, box[bindx].font_indx);
    //Serial.print("Box: "); Serial.print(bindx); Serial.print(" = ");Serial.println(db_box[bindx].txt);
}

char test_txt[] = "Kummeli offers fast, absurd sketches and beloved Finnish oddballs.";
void box_test(void)
{
    for(uint8_t i= 1; i < BOX_NBR_OF-1; i++)
    {
        strncpy(box[i].txt,&test_txt[i-1],BOX_TEXT_LEN);
        box_draw(i);
    }
    delay(2000);

    strncpy(box[11].txt,"-21.2349",BOX_TEXT_LEN);
    box_draw(11);


}

