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

#define T_ONE	100
#define T_ZERO	250

void StripLLInit(void);
void StripLLSet(int channel, rgb *data, int cnt);
void StripLLEnable(int enable);

#endif /* STRIP_LL_H_ */
