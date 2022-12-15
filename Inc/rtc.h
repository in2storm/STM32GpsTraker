/**
  ******************************************************************************
  * File Name          : RTC.h
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
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
#ifndef __rtc_H
#define __rtc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */
#define GPS_ON                  1
#define GPS_OFF                 2
#define NBIoT_ON                3
#define NBIoT_OFF               4
#define GPS_SET                 5
#define NBIoT_SET               6

/* USER CODE BEGIN Private defines */
#define DEFAULT_YEAR            (0x18)
#define DEFAULT_MONTH           (RTC_MONTH_FEBRUARY)
#define DEFAULT_DAY             (0x08)
#define DEFAULT_WEEKDAY         (RTC_WEEKDAY_WEDNESDAY)

#define DEFAULT_HOUR            (0x09)
#define DEFAULT_MINUTE          (0x00)
#define DEFAULT_SECOND          (0x00)
#define DEFAULT_TIME_FORMAT     ((DEFAULT_HOUR >= 0x12) ? RTC_HOURFORMAT12_PM : \
                                                          RTC_HOURFORMAT12_AM)

#define ALARM_HOURS             0
#define ALARM_MINUTES           1

/* USER CODE END Private defines */

extern void _Error_Handler(char *, int);

void MX_RTC_Init();
void RTC_UPDATE(char *rxAckPayload);

/* USER CODE BEGIN Prototypes */
void MX_RTC_PRINT(uint8_t type);
void Alarm_config(void);
void AlarmTest_config(void);
void MX_RTC_DeInit(void);
void MX_RTC_SETTING(uint8_t yy, uint8_t month, uint8_t day, 
                    uint8_t hh, uint8_t mm, uint8_t ss);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ rtc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
