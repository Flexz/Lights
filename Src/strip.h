/*
 * strip.h
 *
 *  Created on: 24 ���. 2020 �.
 *      Author: Flexz
 */

#ifndef STRIP_H_
#define STRIP_H_

#include "colors.h"

void StripInit();

void StripUpdate(int channel, rgb *src, int cnt);

void StripClear();

void StripSet(rgb color);

#endif /* STRIP_H_ */
