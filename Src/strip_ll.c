/*
 * strip_ll.c
 *
 *  Created on: 28 Dec. 2020
 *      Author: Flexz
 */

#include "main.h"
#include "strip_ll.h"
#include "stm32h7xx_hal.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;

static void Dma_Init();
static void TIM1_Init(void);
static void TIM6_Init(void);

static void UpdateByte(volatile uint16_t *ptr, uint8_t value);

static volatile int enabled;
/**
 * Длительности импульсов для каждого разряда
 * Дополнительный разряд в конце - для обнуления линии (импульс нулевой длительности оставит линию в состоянии сброса)
 * Инициализация тут только для того, чтобы массив ушел в сегмент data, который доступен для DMA (секция RAM_D1, см. скрипт линкера)
 */
volatile uint16_t bits1[TOTAL_BITS+1] = {0,1};

void StripLLInit()
{
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM6_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();
	//Инициализация массива
	for(int i = 0; i < TOTAL_BITS; i++)
	{
		bits1[i] = T_ZERO;
	}
	bits1[TOTAL_BITS] = 0;//Добавить импульс нулевой длительности для сброса линии
	enabled = 0;

	Dma_Init();
	TIM1_Init();
	TIM6_Init();
}

void StripLLSet(int channel, rgb *data, int cnt)
{
	for(int i = 0; i < cnt; i++)
	{
		uint8_t v;
		v = data[i].r * 255;
		UpdateByte(bits1 + i*24 + 0, v);
		v = data[i].g * 255;
		UpdateByte(bits1 + i*24 + 8, v);
		v = data[i].b * 255;
		v = (v & 0xFC) + 0;
		UpdateByte(bits1 + i*24 + 16, v);
	}
	bits1[TOTAL_BITS] = 0;
}

void UpdateByte(volatile uint16_t *ptr, uint8_t value)
{
	for(int i = 0; i < 8; i++)
	{
		if(value & (1 << (7-i)))
		{
			ptr[i] = T_ONE;
		}
		else
		{
			ptr[i] = T_ZERO;
		}
	}
}

void StripLLEnable(int enable)
{
	enabled = enable;
	if(enable)
	{
		bits1[TOTAL_BITS] = 0;
		HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)bits1, TOTAL_BITS+1);
	}
}

static void TIM1_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = T_PRESCALER;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = T_PULSE;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  __HAL_TIM_ENABLE_OCxPRELOAD(&htim1, TIM_CHANNEL_1);

  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

void TIM6_Init()
{
	htim6.Instance = TIM6;
	htim6.Init.Prescaler = 200;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = 100;
	htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim6.Init.RepetitionCounter = 0;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_OnePulse_Init(&htim6, TIM_OPMODE_SINGLE) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void Dma_Init()
{
	static DMA_HandleTypeDef  hdma_tim;

	/* Set the parameters to be configured */
	hdma_tim.Init.Request  = DMA_REQUEST_TIM1_CH1;
	hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD ;
	hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD ;
	hdma_tim.Init.Mode = DMA_NORMAL;
	hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_tim.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_tim.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_tim.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_tim.Init.PeriphBurst = DMA_PBURST_SINGLE;

	/* Set hdma_tim instance */
	hdma_tim.Instance = DMA1_Stream0;

	/* Link hdma_tim to hdma[TIM_DMA_ID_CC3] (channel3) */
	__HAL_LINKDMA(&htim1, hdma[TIM_DMA_ID_CC1], hdma_tim);

	/* Initialize TIMx DMA handle */
	HAL_DMA_Init(htim1.hdma[TIM_DMA_ID_CC1]);

	/*##-2- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}

/**
 * Called indirectly from DMA1_Stream0_IRQHandler->HAL_DMA_IRQHandler
 * Will start pause timer TIM6, which will start new DMA cycle in TIM6_DAC_IRQHandler
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	htim6.Instance->CNT = 0;
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim6)
	{
		HAL_TIM_Base_Stop_IT(&htim6);
		bits1[TOTAL_BITS] = 0;

		if(enabled)
			HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)bits1, TOTAL_BITS+1);
	}
}

/**
 * Pause timer
 * Start new DMA cycle
 */
void TIM6_DAC_IRQHandler()
{
	if(__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE))
	{

	}
	HAL_TIM_IRQHandler(&htim6);


}

void DMA1_Stream0_IRQHandler()
{
	HAL_DMA_IRQHandler(htim1.hdma[TIM_DMA_ID_CC1]);
}

