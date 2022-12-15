/**
******************************************************************************
* File Name          : RTC.c
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

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include <stdarg.h>
#include <string.h>
#include "usart.h"
#include "NBIoT.h"

RTC_TimeTypeDef gTime;
RTC_DateTypeDef gDate;
RTC_HandleTypeDef hrtc;

RTC_AlarmTypeDef gAlarm;

#define ALARMTIME_TABLE_SIZE    3 // 3 and 7
//// 7 times, time table
//uint8_t AlarmTime_table[ALARMTIME_TABLE_SIZE] = 
//{
//  {3},  // 0
//  {6}, // 1
//  {9}, // 2
//  {12}, // 3
//  {18}, // 4
//  {21}, // 5
//  {00}, // 6
//};
// 3 times, time table
uint8_t AlarmTime_table[ALARMTIME_TABLE_SIZE] = 
{
  {8},  // 0
  {17}, // 1
  {00}, // 2
};
uint8_t Alarm_NextNum(uint8_t hh);

/* USER CODE END 0 */




/* RTC init function */
void MX_RTC_Init()
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  
  /**Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  /**Initialize RTC and set the Time and Date 
  */
  sDate.Year = DEFAULT_YEAR;
  sDate.Month = DEFAULT_MONTH;
  sDate.Date = DEFAULT_DAY;
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  
  
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  sTime.Hours = DEFAULT_HOUR;
  sTime.Minutes = DEFAULT_MINUTE;
  sTime.Seconds = DEFAULT_SECOND;
  sTime.TimeFormat = RTC_HOURFORMAT12_AM;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{
  
  if(rtcHandle->Instance==RTC)
  {
    /* USER CODE BEGIN RTC_MspInit 0 */
    
    /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
    
    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
    /* USER CODE BEGIN RTC_MspInit 1 */
    
    /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{
  
  if(rtcHandle->Instance==RTC)
  {
    /* USER CODE BEGIN RTC_MspDeInit 0 */
    
    /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */
    
  /* USER CODE END RTC_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

void MX_RTC_DeInit(void)
{
  if (HAL_RTC_DeInit(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  memset( (void *)&hrtc, NULL, sizeof(hrtc) );
}

/**
* @brief  Alarm callback
* @param  RTC handle
* @retval None
*/
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  NBIoT_RTCWakeup_State();
}

/**
* @brief  Alarm config
* @param  none
* @retval none
*/
void Alarm_config(void)
{

  RTC_AlarmTypeDef sAlarm;
 
  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
  uint8_t AlarmTime = AlarmTime_table[Alarm_NextNum(gTime.Hours)];
  uint8_t AlarmTime_hex = 0;
  AlarmTime_hex = (AlarmTime%10) | ((AlarmTime/10)<<4);
  /*##-1- Configure the RTC Alarm peripheral #################################*/
  /* Set Alarm 
  RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
  sAlarm.Alarm = RTC_ALARM_A;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
  sAlarm.AlarmTime.Hours = AlarmTime_hex;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  
  if(HAL_RTC_SetAlarm_IT(&hrtc,&sAlarm,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  HAL_RTC_GetAlarm(&hrtc,&gAlarm,RTC_ALARM_A,RTC_FORMAT_BCD);
  __no_operation();
}

/**
* @brief  AlarmTest config
* @param  none
* @retval none
*/
void AlarmTest_config(void)
{
  RTC_AlarmTypeDef sAlarm;
 
  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
  
  uint8_t AlarmHours = gTime.Hours;
  uint8_t AlarmMinutes = gTime.Minutes;
  uint8_t AlarmSeconds = gTime.Seconds;
    
//  AlarmSeconds = AlarmSeconds + 10;     //Alarm seconds set (30 -> 30second)
  AlarmMinutes = AlarmMinutes + 10;     //Alarm Minutes set (5-> 5minute)
//  AlarmHours = AlarmHours + 1;     //Alarm Hours set (1 -> 1hour)
  
  if( AlarmSeconds >= 60 )
  {
    AlarmSeconds = AlarmSeconds % 60;
    AlarmMinutes = AlarmMinutes + 1;
  }
  if( AlarmMinutes >= 60 )
  {
    AlarmMinutes = AlarmMinutes % 60;
    AlarmHours = AlarmHours + 1;
  }
  if( AlarmHours >= 24)
  {
    AlarmHours = 0;
  }
  
  uint8_t AlarmHours_hex = (AlarmHours%10) | ((AlarmHours/10)<<4);
  uint8_t AlarmMinutes_hex = (AlarmMinutes%10) | ((AlarmMinutes/10)<<4);
  uint8_t AlarmSeconds_hex = (AlarmSeconds%10) | ((AlarmSeconds/10)<<4);
  /*##-1- Configure the RTC Alarm peripheral #################################*/
  /* Set Alarm 
  RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
  sAlarm.Alarm = RTC_ALARM_A;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
  sAlarm.AlarmTime.Hours = AlarmHours_hex;
  sAlarm.AlarmTime.Minutes = AlarmMinutes_hex;
  sAlarm.AlarmTime.Seconds = AlarmSeconds_hex;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  
  if(HAL_RTC_SetAlarm_IT(&hrtc,&sAlarm,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  HAL_RTC_GetAlarm(&hrtc,&gAlarm,RTC_ALARM_A,RTC_FORMAT_BCD);
  __no_operation();
}

/**
* @brief  Alarm table 'AlarmTime_table' index
* @param  get hours
* @retval table index
*/
uint8_t Alarm_NextNum(uint8_t hh)
{
  uint8_t alarmTime_Index = 0;
  
  if(hh >= AlarmTime_table[ALARMTIME_TABLE_SIZE-2]) 
  {
    alarmTime_Index = ALARMTIME_TABLE_SIZE-1; // End time (0h)
  }
  else if(hh < AlarmTime_table[0]) 
  {
    alarmTime_Index = 0;// Start time(8h)
  }
  else
  {
    for( uint8_t i = 0; i < ALARMTIME_TABLE_SIZE-2; i++)
    {
      if( (hh >= AlarmTime_table[i]) && 
         (hh < AlarmTime_table[i+1]))
      {
        alarmTime_Index = i+1;
      }
    }
  }
  
  return alarmTime_Index;
}

/**
* @brief  RTC time print
* @param  type-GPS,NBIot
* @retval none
*/
char aShowTime[255] = {0,};
void MX_RTC_PRINT(uint8_t type)
{
  if( type == GPS_ON )
    USARTx_SendData(&huart4, "GPS_ON Time : ", strlen("GPS_ON Time : "));
  else if( type == GPS_OFF )
    USARTx_SendData(&huart4, "GPS_OFF Time : ", strlen("GPS_OFF Time : "));
  else if( type == NBIoT_ON )
    USARTx_SendData(&huart4, "NBIoT_ON Time : ", strlen("NBIoT_ON Time : "));
  else if( type == NBIoT_OFF )
    USARTx_SendData(&huart4, "NBIoT_OFF Time : ", strlen("NBIoT_OFF Time : "));
  else if( type == NBIoT_SET )
    USARTx_SendData(&huart4, "NBIoT_SET Time : ", strlen("NBIoT_SET Time : "));
  else if( type == GPS_SET )
    USARTx_SendData(&huart4, "GPS_SET Time : ", strlen("GPS_SET Time : "));
  
  memset(aShowTime, 0x00, sizeof(aShowTime));
  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
  
  sprintf(aShowTime, "%04d/%02d/%02d / %02d:%02d:%02d\r\n",
          gDate.Year+2000, gDate.Month, gDate.Date,
          gTime.Hours, gTime.Minutes, gTime.Seconds);
  USARTx_SendData(&huart4, aShowTime, strlen(aShowTime));
}

/**
* @brief  RTC Set
* @param  year, month, day, hours, minutes, seconds
* @retval none
*/
void MX_RTC_SETTING(uint8_t yy, uint8_t month, uint8_t day, uint8_t hh, uint8_t mm, uint8_t ss)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
  
  /**Initialize RTC and set the Time and Date */
  sDate.Year    = yy;
  sDate.Month   = month;
  sDate.Date    = day;
  sDate.WeekDay = DEFAULT_WEEKDAY;
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  sTime.Hours   = hh;
  sTime.Minutes = mm;
  sTime.Seconds = ss;
  sTime.TimeFormat = DEFAULT_TIME_FORMAT;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

/**
* @brief  RTC Update
* @param  UDP Ack payload
* @retval none
*/
void RTC_UPDATE(char *rxAckPayload)
{
  uint8_t YY = 0;
  uint8_t MM = 0;
  uint8_t DD = 0;
  uint8_t hh = 0;
  uint8_t mm = 0;
  uint8_t ss = 0;
  uint8_t copyPayload[56] = {0,};
  
  memcpy(copyPayload, rxAckPayload, sizeof(copyPayload));
  
  YY = ((copyPayload[31]-'0')<<4) | (copyPayload[33]-'0');
  MM = ((copyPayload[35]-'0')<<4) | (copyPayload[37]-'0');
  DD = ((copyPayload[39]-'0')<<4) | (copyPayload[41]-'0');
  hh = ((copyPayload[43]-'0')<<4) | (copyPayload[45]-'0');
  mm = ((copyPayload[47]-'0')<<4) | (copyPayload[49]-'0');
  ss = ((copyPayload[51]-'0')<<4) | (copyPayload[53]-'0');
  
  MX_RTC_SETTING(YY, MM, DD, hh, mm, ss);
}
/* USER CODE END 1 */

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
