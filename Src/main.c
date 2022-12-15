/**
******************************************************************************
* File Name          : main.c
* Description        : Main program body
******************************************************************************
** This notice applies to any and all portions of this file
* that are not between comment pairs USER CODE BEGIN and
* USER CODE END. Other portions of this file, whether 
* inserted by the user or by software development tools
* are owned by their respective copyright owners.
*
* COPYRIGHT(c) 2017 STMicroelectronics
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "stm32l4xx_hal.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"
#include "W25Qx.h"
#include "NBIoT.h"
#include "GPS.h"
/* USER CODE BEGIN define */
#include "usart.h"
#include "vcom.h" //AidLabs added 2019.04.10
#include "bma400.h" //AidLabs added 2019.04.19
#include "bme280.h" //AidLabs added 2019.04.19

#define GPS_WAIT_COUNT           90 // GPS_WAIT_COUNT * 1s
/* USER CODE END define */
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE END PFP */

uint8_t mainState = GPS_STATE;
_typeGPS_State GPS_S;

int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();
  
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  
  MX_I2C1_Init();
  MX_SPI1_Init();
  
  gpio_Config();
  uart_Config();
  NBIoT_UDP_Init();
  
  //PRINTF("NB-IoT Power on\r\n");  
  //BMA400_initialize();
  //PRINTF("BMA400 init ok\r\n");
  //BME280_init();
  //PRINTF("BME280 init ok\r\n");
  /*while(1){
       
  
    PRINTF("CGMI command\r\n");
    NBIoT_SEND_CMD("AT+CGMI\r\n",MAX_SUMMER_USART_TIMEOUT);
    NBIoT_TIMEOUT(1000*5);
    //PRINTF("NB-IoT Power off\r\n");
    //NBIoT_PowerOff();  
  }*/    
  /* NBIot intitialize timeout */
 // NBIoT_TIMEOUT(1000*60);  
 // PRINTF("NBIOT Timout end\r\n");
  //NBIoT_SEND_CMD("AT+NCONFIG=CELL_RESELECTION,TRUE\r\n",MAX_SUMMER_USART_TIMEOUT);
  //uint8_t aTxBuffer[8] = {0xaa,};
  //uint8_t aRxBuffer[8] = {0x0,};
#if 0
  float x = 0, y = 0, z = 0;
  while(1){    
    getAcceleration(&x,&y,&z);
    PRINTF("x = %f, y = %f, z = %f \r\n", x, y, z);
    
   // PRINTF("temp = %2.2f[C], pressure = %f[Pa], altitude = %2.2f[m], Humidity[%%RH] = %2.2f[%%]\r\n",BME280readTempC(),BME280readFloatPressure(),BME280readFloatAltitudeMeters(),BME280readFloatHumidity());

    HAL_Delay(1000); 
  }
#endif
  /* Infinite loop */
  while (1)
  {
    /** Main State **/
    switch(mainState)
    {
    case GPS_STATE:
      /* GPS Timeout */
      if( GPS_S.GPS_Set == 1) // GPS Ready 
      {
        NBIoT_GPSData_Save();
      }
      else if(GPS_S.GPS_SetCount == GPS_WAIT_COUNT) // GPS Timeout
      {
        NBIoT_GPSData_Save();
      }
      else //GPS Timeout Count
      {
        uint8_t printf_len = 0;
        GPS_S.GPS_SetCount++;
        printf_len = sprintf(GPS_S.GPSPrint_Buf, "GPS Timeout Count(s) : %d\r\n", GPS_S.GPS_SetCount);
        USARTx_SendData(&huart4, GPS_S.GPSPrint_Buf, printf_len);
      }
      HAL_Delay(1000); //1s delay
      break;
      
    case NBIOT_STATE:
      /* NBIot Process */
      NBIoT_Init_Process(); 
      break;
    case FLASH_STATE: 
      
      break;
    case SENSOR_STATE: 
      
      break;
    case SLEEP_STATE:
      /* MCU Sleep set */
      sleep_set();
      /* MCU Wakeup set */
      wakeup_set();
      mainState = GPS_STATE;
      break;
    }
  }
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
  
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  
  /**Configure LSE Drive Capability 
  */
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
    |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3
      |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_I2C1
        |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /**Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /**Configure the Systick interrupt time 
  */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  
  /**Configure the Systick 
  */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
* @brief  MCU Wakeup setting
* @param  none
* @retval none
*/
void wakeup_set(void)
{
  /* System clock config */
  SystemClock_Config();
  
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_UART4_Init();
  
  GPS_PowerOn();
  uart_Config();
}

/**
* @brief  MCU Sleep setting
* @param  none
* @retval none
*/
void sleep_set(void)
{
  Alarm_config(); //Alarm set
//  AlarmTest_config(); //Test alarm set
  
  GPS_S.GPS_Set = 0;
  GPS_S.GPS_SetCount = 0;
  /* Sleep time print */
  USARTx_SendData(&huart4, "\r\nMCU SLEEP\r\n", strlen("\r\nMCU SLEEP\r\n"));
  MX_RTC_PRINT(0);
  
  HAL_Delay(10);
  HAL_UART_DeInit(&huart4);
  HAL_UART_DeInit(&huart1);
  HAL_UART_DeInit(&huart3);
  
  /* NBIot Pin Input set */
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_DeInit(GPIOB,NBIoT_POWER_Pin);
  HAL_GPIO_DeInit(GPIOB,NBIoT_MCU_RESET_Pin);
  HAL_GPIO_DeInit(GPIOB,NBIoT_RESET_Pin);
  
  GPIO_InitStruct.Pin = NBIoT_MCU_RESET_Pin|NBIoT_RESET_Pin|NBIoT_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Power down STOP2 mode */
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
  
  
}

static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00D00E28l;//0x00000E14;  
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0x0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

}
/**
* @brief  This function is executed in case of error occurrence.
* @param  None
* @retval None
*/
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
* @brief Reports the name of the source file and the source line number
* where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
  
}

#endif

/**
* @}
*/ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
