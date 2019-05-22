/*
 * graphic.h
 *
 *  Created on: Apr 17, 2019
 *      Author: valmit
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include "stm32f0xx.h"

void set_buffer(uint8_t* new_buffer);

void init_graphic(void);

void pixel(uint32_t n, uint8_t leds);

void draw_buffer();

#endif /* GRAPHIC_H_ */
