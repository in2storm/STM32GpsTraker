/**
******************************************************************************
* File Name          : gpio.c
* Description        : This file provides code for the configuration
*                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */
#include "rtc.h"
#include "i2c.h"
#include "spi.h"
#include "W25Qx.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
* Analog 
* Input 
* Output
* EVENT_OUT
* EXTI
*/
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /*Configure GPIO pins : PBPin */
  GPIO_InitStruct.Pin = GPS_1PPS_Pin|GPS_POWER_Pin|NBIoT_POWER_Pin|NBIoT_RESET_Pin|NBIoT_MCU_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  
  
  
  GPIO_InitStruct.Pin = GPIO_PIN_4;//SPI1 CS for BMA400
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  
  
    /*Configure GPIO pin : PB15 BMA400 INT2*/ 
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC6 BMA400 INT1*/
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
}

/**
* @brief  gpio set
* @param  None
* @retval None
*/
void gpio_Config()
{
  /* NBIoT GPIO Set */
  NBIoT_PowerOn();  
  /* GPS GPIO Set */
  GPS_PowerOn();
}

/**
* @brief  NBIot power on set
* @param  None
* @retval None
*/
void NBIoT_PowerOn(void)
{
  HAL_GPIO_WritePin(NBIoT_RESET_GPIO_Port, NBIoT_RESET_Pin, GPIO_PIN_RESET);
  /* NBIoT Power on set */
  HAL_GPIO_WritePin(NBIoT_POWER_GPIO_Port, NBIoT_POWER_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(NBIoT_MCU_RESET_GPIO_Port, NBIoT_MCU_RESET_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(NBIoT_RESET_GPIO_Port, NBIoT_RESET_Pin, GPIO_PIN_SET);
  /* RTC Print */
  MX_RTC_PRINT(NBIoT_ON);
}

/**
* @brief  NBIot power off set
* @param  None
* @retval None
*/
void NBIoT_PowerOff(void)
{
  /* NBIoT Power off set */
  HAL_GPIO_WritePin(NBIoT_POWER_GPIO_Port, NBIoT_POWER_Pin, GPIO_PIN_RESET);
  //HAL_GPIO_WritePin(NBIoT_MCU_RESET_GPIO_Port, NBIoT_MCU_RESET_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(NBIoT_RESET_GPIO_Port, NBIoT_RESET_Pin, GPIO_PIN_RESET);
  /* RTC Print */
  MX_RTC_PRINT(NBIoT_OFF);
}

/**
* @brief  GPS power on set
* @param  None
* @retval None
*/
void GPS_PowerOn(void)
{
  /* GPS Power on set */
  HAL_GPIO_WritePin(GPS_POWER_GPIO_Port, GPS_POWER_Pin, GPIO_PIN_SET);
  /* RTC Print */
  MX_RTC_PRINT(GPS_ON);
}

/**
* @brief  GPS power off set
* @param  None
* @retval None
*/
void GPS_PowerOff(void)
{
  /* GPS Power off set */
  HAL_GPIO_WritePin(GPS_POWER_GPIO_Port, GPS_POWER_Pin, GPIO_PIN_RESET);
  /* RTC Print */
  MX_RTC_PRINT(GPS_OFF);
}

/* USER CODE END 2 */

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
