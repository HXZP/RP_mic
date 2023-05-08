/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "stdio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t get;

typedef struct high_pass{

	float out;
	float out_pre;
	
	float in;
	float in_pre;

	float a;
}high_pass_t;
high_pass_t pass;

float out_adc;


#define FFT_LENGTH 1024
//float fft_inputbuf[FFT_LENGTH*2]; //FFT 输入数组
//float fft_outputbuf[FFT_LENGTH]; //FFT 输出数组

#define LEN 256
uint32_t adc_input[LEN];

//float fft_inputbuf2[FFT_LENGTH*2]; //FFT 输入数组
//float fft_outputbuf2[FFT_LENGTH]; //FFT 输出数组


uint32_t time1,time2,time_f;


typedef struct time_c{

	uint32_t time1;
	uint32_t time2;
	uint32_t time_f;

}time_t;


uint32_t us(void)
{
	
	return TIM2->CNT+HAL_GetTick()*1000;

}

int fputc(int ch, FILE *f)
 
{
 
  HAL_UART_Transmit(&huart5, (uint8_t *)&ch, 1, 0xffff);
 
  return ch;
 
}


uint16_t adc_flag = 0;
void Get_Adc(void)
{
	
	HAL_ADC_PollForConversion(&hadc1,10);
	
	get = HAL_ADC_GetValue(&hadc1);


}

char adc_f = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	time2 = time1;
	time1 = TIM2->CNT+HAL_GetTick()*1000;
	
	time_f = time1-time2;
	
	adc_f = 1;	
	
	HAL_ADC_Stop_DMA(&hadc1);

}


time_t time_p;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	arm_cfft_radix4_instance_f32 scfft;
	float s=0.001;
	uint8_t d = 1;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_UART5_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1,adc_input,LEN);
	
	
	arm_cfft_radix4_init_f32(&scfft,2,0,1);

	adc_flag = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		if(adc_f == 1)
		{
			time_p.time1 = HAL_GetTick();
			for(uint16_t i = 0; i < LEN; i++){
			
				s = adc_input[i];
				printf("%f\n",s);
		
			}
			printf("00\n");
			
			time_p.time_f = HAL_GetTick() - time_p.time1;;
			
			adc_f = 0;
			HAL_ADC_Start_DMA(&hadc1,adc_input,LEN);
		
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
//		time1 = TIM2->CNT+HAL_GetTick()*1000;
//		
//		HAL_ADC_PollForConversion(&hadc1,10);
//		get = HAL_ADC_GetValue(&hadc1);
		
//		fft_inputbuf[2*adc_flag] = get;
//		fft_inputbuf[2*adc_flag + 1] = 0;

//		adc_flag++;
		
//		while(TIM2->CNT+HAL_GetTick()*1000 - time1 < 100);
//		s = get;
//		printf("%f\n",s);
//		if(adc_flag == FFT_LENGTH){
//		
//			for(uint16_t i = 0; i < FFT_LENGTH; i++){
//			
//				s = fft_inputbuf[2*i];
//				printf("%f\n",s);
//				
//			}
//			adc_flag = 0;
//		}

//		HAL_UART_Transmit(&huart5, &d, 1, 0xffff);
		
//		time1 = TIM2->CNT+HAL_GetTick()*1000;
//		
////		HAL_ADC_PollForConversion(&hadc1,10);
////		get = HAL_ADC_GetValue(&hadc1);
//		printf("%f\n",s);
//		
//		time2 = TIM2->CNT+HAL_GetTick()*1000;
//		time_f = time2 - time1;
		
		
//		pass.in_pre = pass.in;
//		
//		pass.in = get;
//		
//		pass.out_pre = pass.out;
//		
//		pass.out = pass.a*(pass.in - pass.in_pre) + pass.a * pass.out_pre;
//		
//		out_adc = pass.out;
		
		
		
//		for(int i=0;i<FFT_LENGTH;i++)//生成信号序列
//		{
//			fft_inputbuf[2*i]=100+ 
//				10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
//				30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
//				50*arm_cos_f32(2*PI*i*8/FFT_LENGTH); //实部
//			
//			fft_inputbuf[2*i+1]=0;//虚部全部为 0
//		}		

//		for(int i=0;i<FFT_LENGTH;i++)//生成信号序列
//		{
//			fft_inputbuf2[2*i]=100+ 
//				10*arm_sin_f32(2*PI*i/FFT_LENGTH)+
//				30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+
//				50*arm_cos_f32(2*PI*i*8/FFT_LENGTH); //实部
//			
//			fft_inputbuf2[2*i+1]=0;//虚部全部为 0
//		}		
		
//		time1 = TIM2->CNT+HAL_GetTick()*1000;
//		
//		arm_cfft_radix4_f32(&scfft,fft_inputbuf);
//		arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);

////		arm_cfft_radix4_f32(&scfft,fft_inputbuf2);
////		arm_cmplx_mag_f32(fft_inputbuf2,fft_outputbuf2,FFT_LENGTH);
//		
//		time2 = TIM2->CNT+HAL_GetTick()*1000;
//		time_f = time2 - time1;
		
		
    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
