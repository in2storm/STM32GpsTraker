/**
  ******************************************************************************
  * File Name          : main.hpp
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "intrinsics.h"
#include <stdint.h>

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BLE_RESET_Pin GPIO_PIN_13
#define BLE_RESET_GPIO_Port GPIOC
#define BAT_ADC_Pin GPIO_PIN_0
#define BAT_ADC_GPIO_Port GPIOC
#define BAT_ADC_CTRL_Pin GPIO_PIN_1
#define BAT_ADC_CTRL_GPIO_Port GPIOC
#define DEBUG_UART_TX_Pin GPIO_PIN_0
#define DEBUG_UART_TX_GPIO_Port GPIOA
#define DEBUG_UART_RX_Pin GPIO_PIN_1
#define DEBUG_UART_RX_GPIO_Port GPIOA
#define BLE_UART_TX_Pin GPIO_PIN_2
#define BLE_UART_TX_GPIO_Port GPIOA
#define BLE_UART_RX_Pin GPIO_PIN_3
#define BLE_UART_RX_GPIO_Port GPIOA
#define W25Q_SPI1_CS_Pin GPIO_PIN_4
#define W25Q_SPI1_CS_GPIO_Port GPIOA
#define W25Q_SPI1_SCK_Pin GPIO_PIN_5
#define W25Q_SPI1_SCK_GPIO_Port GPIOA
#define W25Q_SPI1_MISO_Pin GPIO_PIN_6
#define W25Q_SPI1_MISO_GPIO_Port GPIOA
#define W25Q_SPI1_MOSI_Pin GPIO_PIN_7
#define W25Q_SPI1_MOSI_GPIO_Port GPIOA
#define GPS_UART_TX_Pin GPIO_PIN_4
#define GPS_UART_TX_GPIO_Port GPIOC
#define GPS_UART_RX_Pin GPIO_PIN_5
#define GPS_UART_RX_GPIO_Port GPIOC
#define GPS_1PPS_Pin GPIO_PIN_0
#define GPS_1PPS_GPIO_Port GPIOB
#define W25Q_RESET_Pin GPIO_PIN_10
#define W25Q_RESET_GPIO_Port GPIOB
#define W25Q_WP_Pin GPIO_PIN_11
#define W25Q_WP_GPIO_Port GPIOB
#define BLE_IO_Pin GPIO_PIN_12
#define BLE_IO_GPIO_Port GPIOB
#define BMX_ACL_INT1_Pin GPIO_PIN_15
#define BMX_ACL_INT1_GPIO_Port GPIOB
#define BMX_ACL_INT2_Pin GPIO_PIN_6
#define BMX_ACL_INT2_GPIO_Port GPIOC
#define BMX_GYR_INT3_Pin GPIO_PIN_7
#define BMX_GYR_INT3_GPIO_Port GPIOC
#define BMX_GYR_INT4_Pin GPIO_PIN_8
#define BMX_GYR_INT4_GPIO_Port GPIOC
#define BMX_MGN_INT5_Pin GPIO_PIN_9
#define BMX_MGN_INT5_GPIO_Port GPIOC
#define BMX_CS_Pin GPIO_PIN_8
#define BMX_CS_GPIO_Port GPIOA
#define BMX_SCL_Pin GPIO_PIN_9
#define BMX_SCL_GPIO_Port GPIOA
#define BMX_SDA_Pin GPIO_PIN_10
#define BMX_SDA_GPIO_Port GPIOA
#define GPS_POWER_Pin GPIO_PIN_4
#define GPS_POWER_GPIO_Port GPIOB
#define NBIoT_POWER_Pin GPIO_PIN_5
#define NBIoT_POWER_GPIO_Port GPIOB
#define NBIoT_UART_TX_Pin GPIO_PIN_6
#define NBIoT_UART_TX_GPIO_Port GPIOB
#define NBIoT_UART_RX_Pin GPIO_PIN_7
#define NBIoT_UART_RX_GPIO_Port GPIOB
#define NBIoT_RESET_Pin GPIO_PIN_8
#define NBIoT_RESET_GPIO_Port GPIOB
#define NBIoT_MCU_RESET_Pin GPIO_PIN_9
#define NBIoT_MCU_RESET_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
// Main State
#define GPS_STATE               1
#define NBIOT_STATE             2
#define FLASH_STATE             3
#define SENSOR_STATE            4
#define SLEEP_STATE             5

typedef struct {
  uint8_t GPS_Set;              // GPS Ready
  uint16_t GPS_SetCount;         // GPS Timeout Count
  
  char GPSPrint_Buf[100];       // GPS log Print buffer
}_typeGPS_State;


void SystemClock_Config(void);
void sleep_set(void);
void wakeup_set(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
