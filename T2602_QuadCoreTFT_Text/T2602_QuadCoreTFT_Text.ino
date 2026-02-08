/*
 An example analogue meter using a HX8357 TFT LCD screen

 Needs Font 2 (also Font 4 if using large scale label)

 Make sure all the required fonts are loaded by editing the
 User_Setup.h file in the TFT_HX8357_Due library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  ######       TO SELECT THE FONTS AND PINS YOU USE, SEE ABOVE       ######
  #########################################################################
 
Updated by Bodmer for variable meter size
 */

// Define meter size as multiplier of 240 pixels wide 1.0 and 1.3333 work OK
#define M_SIZE 1.3333
#include "io.h"
#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library
#include "box.h"

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB
//#define PIN_LDR_ANALOG_INP    (26u)
//#define PIN_PIR_INP           (5u)
#define PIN_TFT_LED_OUT       (10u)

float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = M_SIZE*120, osy = M_SIZE*120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update

int old_analog =  -999; // Value last displayed

int value[6] = {0, 0, 0, 0, 0, 0};
int old_value[6] = { -1, -1, -1, -1, -1, -1};
int d = 0;

void setup(void) {
  tft.init();
  tft.setRotation(1);
  Serial.begin(115200); // For debug
  
  pinMode(PIN_TFT_BL, OUTPUT);
  digitalWrite(PIN_TFT_BL,HIGH);
  tft.fillScreen(TFT_RED);
  box_initialize();
  box_test();
  analogReadResolution(12);
  //analogWrite(PIN_TFT_LED_OUT,255);

  //analogMeter(); // Draw analogue meter

  updateTime = millis(); // Next update time
}


void loop() {
  if (false)
  {
       // (updateTime <= millis()) {
    updateTime = millis() + 35; // Update meter every 35 milliseconds
  }
}

