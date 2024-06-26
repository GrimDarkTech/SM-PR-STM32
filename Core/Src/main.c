
#include "main.h"

TIM_HandleTypeDef htim1;

void SystemClock_Config(void);

static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);

void WriteInt2Bits(int value);


int value = 0;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM1_Init();

//  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  int encoderA = -1;
  int encoderB = -1;

  int prevA = -1;
  int prevB = -1;

  int dutyCycle = 0;

//  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  while (1)
  {
	  prevA = encoderA;
	  prevB = encoderB;

	  encoderA = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
	  encoderB = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);

	  if(encoderA == 0 && encoderB == 1)
	  {
		  if(prevA == 1 && prevB == 1)
		  {
			  value++;
		  }
		  else if (prevA == 0 && prevB == 0)
		  {
			  value--;
		  }
	  }
	  else if(encoderA == 0 && encoderB == 0)
	  {
		  if(prevA == 0 && prevB == 1)
		  {
			  value++;
		  }
		  else if (prevA == 1 && prevB == 0)
		  {
			  value--;
		  }
	  }
	  else if(encoderA == 1 && encoderB == 0)
	  {
		  if(prevA == 0 && prevB == 0)
		  {
			  value++;
		  }
		  else if (prevA == 1 && prevB == 1)
		  {
			  value--;
		  }
	  }
	  else if(encoderA == 1 && encoderB == 1)
	  {
		  if(prevA == 1 && prevB == 0)
		  {
			  value++;
		  }
		  else if (prevA == 0 && prevB == 1)
		  {
			  value--;
		  }
	  }


	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5))
	  {
		  WriteInt2Bits(value);
	  }
	  else
	  {
		  if(value < 0)
		  {
			  value = 50;
		  }

		  while(value > 0)
		  {
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
				  HAL_Delay(5);

				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
				  HAL_Delay(5);

				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
				  HAL_Delay(5);

				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
				  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 1);
				  HAL_Delay(5);

			  value--;

			  WriteInt2Bits(value);
		  }

		  HAL_Delay(100);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);
	  }


  }
}

void WriteInt2Bits(int value)
{
	int binary[6] = {0, 0, 0, 0, 0, 0};

	int i = 0;

	if(value > 63)
		value = 63;
	if(value < 0)
		value = 0;

	while(value > 0)
	{
		binary[i] = value % 2;
		value = value / 2;
		i++;
	}

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, binary[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, binary[1]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, binary[2]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, binary[3]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, binary[4]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, binary[5]);
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};


  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  __HAL_RCC_TIM1_CLK_ENABLE();


// Read

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

// Write

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, 0);

  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void MX_TIM1_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
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
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim1);

}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {

  }
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
