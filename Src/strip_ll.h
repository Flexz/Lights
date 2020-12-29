/*
 * strip_ll.h
 *
 *  Created on: 28 дек. 2020 г.
 *      Author: Flexz
 */

#ifndef STRIP_LL_H_
#define STRIP_LL_H_

#include "config.h"
#include "colors.h"

#define BITS_PER_LED	24
//#define LED_COUNT		100
#define TOTAL_BITS		(BITS_PER_LED * CFG_STRIP_LEDS)

//500 ticks -> 2.5us
#define T_PULSE	500
//#define T_PULSE	250
//#define T_ONE	125
//#define T_ZERO	50 //75 / 500 =


#define T_ONE	250 //100 / 500 * 2.5 = 0.5us
#define T_ZERO	100 //250 / 500 * 2.5 = 1.25us
//#define T_ONE	170
//#define T_ZERO	80 //75 / 500 =


void StripLLInit(void);
void StripLLSet(int channel, rgb *data, int cnt);
void StripLLEnable(int enable);

#endif /* STRIP_LL_H_ */
