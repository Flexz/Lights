/*
 * strip.h
 *
 *  Created on: 24 дек. 2020 г.
 *      Author: Flexz
 */

#ifndef STRIP_H_
#define STRIP_H_

#include "colors.h"

#define BITS_PER_LED	24
#define LED_COUNT		100
#define TOTAL_BITS		(BITS_PER_LED * LED_COUNT)

#define T_ONE	100
#define T_ZERO	250


void StripInit();

void StripUpdate(rgb *src, int cnt);

void StripClear();

void StripSet(rgb color);

#endif /* STRIP_H_ */
