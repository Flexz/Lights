/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "strip.h"
#include "config.h"
/* USER CODE END Includes */


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
int raibowOffset = 0;
/* USER CODE BEGIN 0 */
void Rainbow(int speed)
{
	hsv hcv_color;
	hcv_color.h = 0.0;
	hcv_color.s = 1.0;
	hcv_color.v = 0.1;

	for(int i = 0; i < CFG_STRIP_LEDS; i++)
	{
		int v = (i*5) + raibowOffset;
		while(v > 360)
			v -= 360;
		hcv_color.h = v;
		StripSetLed(0, i, hsv2rgb(hcv_color));
	}
	raibowOffset += speed;
	if(raibowOffset > 360 * 3)
		raibowOffset -= 360;
}

#define STAR_TAIL	8
int shootingStarStep = 0;
void ShootingStar()
{
	hsv hcv_color;
	hcv_color.h = 0.0;
	hcv_color.s = 1.0;
	hcv_color.v = 0.0;

	StripClear(hsv2rgb(hcv_color));

	hcv_color.h = 0.0;
	hcv_color.s = 0.0;
	hcv_color.v = 0.5;
	for(int i = shootingStarStep; i > shootingStarStep - STAR_TAIL; i--)
	{
		hcv_color.v -= 0.05;
		StripSetLed(0, i, hsv2rgb(hcv_color));
	}
	shootingStarStep++;
	if(shootingStarStep > 100)
		shootingStarStep = 0;
}

int fireworkStep = 0;
#define FW_SETOFF	48
#define FW_BLOW 	60
#define FW_LENGTH	40
int wave[FW_LENGTH] =
{20, 40, 60, 80, 100, 100, 100, 100, 100, 90,
 80, 70, 60, 50, 40,  30,  20,  10,  10,  10,
 10, 10, 10, 10, 10,  10,  10,  10,  10,  10,
 10, 9, 8, 7, 6,  5,  4,  3,  2,  1,
};

void Firework()
{
	hsv hcv_color;
	hcv_color.h = 0.0;
	hcv_color.s = 1.0;
	hcv_color.v = 0.0;

	StripClear(hsv2rgb(hcv_color));
	if(fireworkStep < FW_SETOFF)
	{
		//shoot
		HAL_Delay(25);
		hcv_color.h = 0.0;
		hcv_color.s = 0.0;
		hcv_color.v = 0.5;
		for(int i = fireworkStep; i > fireworkStep - STAR_TAIL; i--)
		{
			hcv_color.v -= 0.8 / STAR_TAIL;
			StripSetLed(0, i, hsv2rgb(hcv_color));
		}
	}
	else if(fireworkStep < FW_BLOW)
	{
		HAL_Delay(50);
		hcv_color.h = 0.0;
		hcv_color.s = 0.1;
		hcv_color.v = 1.0;
		float v = 1.2 - (fireworkStep - FW_SETOFF) / 20.0;

		for(int i = CFG_STRIP_LEDS-1; i >= CFG_STRIP_LEDS-STAR_TAIL*2; i--)
		{
			v -= 0.05;
			hcv_color.v = v;
			if(v > 1.0)
				v = 1.0;
			StripSetLed(0, i, hsv2rgb(hcv_color));
		}
	}
	else
	{
		hcv_color.h = 0.0;
		hcv_color.s = 1.0;
		hcv_color.v = 0.2;
		HAL_Delay(50);
		
		for(int i = CFG_STRIP_LEDS-1; i >= 0; i--)
		{
			int idx = CFG_STRIP_LEDS-i;//from top
			hcv_color.h = idx * 360 / CFG_STRIP_LEDS;	
			int waveIdx = (fireworkStep - FW_SETOFF) - idx;
			if(waveIdx >= FW_LENGTH)
				hcv_color.v = 0;
			else if(waveIdx < 0)
			    hcv_color.v = 0;
			else 
				hcv_color.v = (float)(wave[waveIdx]) / 500;
			StripSetLed(0, i, hsv2rgb(hcv_color));
		}
	}
	fireworkStep++;
	if(fireworkStep > 150)
		fireworkStep = 0;
}

void Flag()
{
	rgb color;
	color = rgb_create(0.3, 0, 0);

	int i = 0;
	for(i = 0; i < CFG_STRIP_LEDS / 3; i++)
	{
		StripSetLed(0, i, color);
	}

	color = rgb_create(0, 0, 0.3);
	for(; i < 3 * CFG_STRIP_LEDS / 4; i++)
	{
		StripSetLed(0, i, color);
	}

	color = rgb_create(0.3, 0.3, 0.3);
	for(; i < CFG_STRIP_LEDS; i++)
	{
		StripSetLed(0, i, color);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();


  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  StripInit();
  StripLLEnable(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  hsv hcv_color;
  hcv_color.h = 0.0;
  hcv_color.s = 1.0;
  hcv_color.v = 0.1;

  int num = 4;
  int buttonTrg = 0;
  int pulse = 0;
  while (1)
  {
	  if(HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET)
	  {
		  if(buttonTrg == 4)
		  {
			  num++;
			  if(num > 5)
				  num = 0;
		  }
		  buttonTrg++;
	  }
	  else
	  {
		  buttonTrg = 0;
	  }
	  switch(num)
	  {
	  case 0:
		  StripClear(hsv2rgb(hcv_color));
		  hcv_color.h += 0.5;
		  if(hcv_color.h >= 360)
			  hcv_color.h = 0;
		  break;
	  case 1:
		  Rainbow(2);
		  break;
	  case 2:
		  Rainbow(5);
		  break;
	  case 3:
		  ShootingStar();
		  break;
	  case 4:
		  Firework();
		  break;
	  case 5:
		  Flag();
		  break;
	  }
	  StripUpdate();

    /* USER CODE END WHILE */
	  HAL_Delay(20);
	  if(pulse)
	  {
		  pulse = 0;
		  HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, LD3_Pin, GPIO_PIN_SET);
	  }
	  else
	  {
		  pulse = 1;
		  HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, LD3_Pin, GPIO_PIN_RESET);
	  }


    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**TIM1 GPIO Configuration
  PE9     ------> TIM1_CH1
  */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  //GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
