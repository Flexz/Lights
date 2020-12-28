/*
 * strip.c
 *
 *  Created on: 24 дек. 2020 г.
 *      Author: Flexz
 */

#include "stm32h7xx_hal.h"
#include "strip.h"
#include "strip_ll.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;

static void Dma_Init();
static void TIM1_Init(void);
static void TIM6_Init(void);

//uint16_t *bitsCurrent = bits1;
rgb strip[CFG_STRIP_CHANNELS][CFG_STRIP_LEDS];

void StripInit()
{
	StripLLInit();
}

void LedSet(int channel, int idx, rgb color)
{
	strip[channel][idx] = color;
	/*uint16_t *arr = bitsCurrent;
	UpdateByte(arr + 3*idx + 0, color.r);
	UpdateByte(arr + 3*idx + 1, color.g);
	UpdateByte(arr + 3*idx + 2, color.b);*/
}

void StripUpdate(int channel, rgb *src, int cnt)
{
	if(cnt > CFG_STRIP_LEDS)
		cnt = CFG_STRIP_LEDS;
	for(int i = 0; i < cnt; i++)
	{
		LedSet(channel, i, src[i]);
	}
}

void StripClear()
{
	rgb clearColor;
	clearColor.r = clearColor.g = clearColor.b = 0;
	StripSet(clearColor);
}

void StripSet(rgb color)
{
	rgb clearColor;
	clearColor.r = clearColor.g = clearColor.b = 0;
	for(int ch = 0; ch < CFG_STRIP_CHANNELS; ch++)
	{
		for(int idx = 0; idx < CFG_STRIP_LEDS; idx++)
		{
			LedSet(ch, idx, color);
		}
	}
}
