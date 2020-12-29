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

//200Mhz / (Prescaler+1) / Pulse
//200M / 1 / 250 = 800.000, Period - 1.25uS, 5nS per tick
#define T_PULSE	250 //
#define T_PRESCALER 0

#define T_ONE	120 //120*5 = 600 (1200 / 2)
#define T_ZERO	50 //50*5 = 250 (500 / 2)


void StripLLInit(void);
void StripLLSet(int channel, rgb *data, int cnt);
void StripLLEnable(int enable);

#endif /* STRIP_LL_H_ */
