#ifndef __BOX_H__
#define __BOX_H__

#include "main.h"
#include <TFT_eSPI.h> 

void box_clear(void);
void box_initialize(void);
void box_draw(uint8_t bindx);
void box_test(void);

#endif