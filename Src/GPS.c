/**
******************************************************************************
* File Name          : GPS.c
* Description        : This file provides code for the configuration
*                      of the GPS instances.
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/


/* USER CODE BEGIN 0 */
#include <stdarg.h>
#include <string.h>
#include "usart.h"
#include "gpio.h"
#include "NBIoT.h"
#include "GPS.h"
#include "tim.h"
#include "rtc.h"

bool CalcChecksum(char *data, uint8_t len);
extern _typeGPS_State GPS_S;

_typeGPS_UART GPS_UARTxM = 
{
  RESET,
  RESET,
  .String = {0,0,},
  0,
  0,
  0,
};

_typeGPSData GPS_DATA = 
{
  {0,},
  {0,},
  {0,},
  0,
  {0,},
};

enum{
  FALSE = 0,
  TRUE,
};

/* USER CODE END 0 */
/**
* @brief  GPS UART CheckSum Return
* @param  CheckSum data
* @retval checksum
*/
bool CalcChecksum(char *data, uint8_t len)
{
  static char szChecksum[2];
  char sum = 0;
  
  sum = data[0];
  for(int i = 1; i <= len; i++) {
    sum = sum ^ data[i];
  }
  
  sprintf(szChecksum, "%02X", sum);
  if( (szChecksum[0] == data[len+2]) &&
     (szChecksum[1] == data[len+3]) )
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief  GPS UART RX Callback
* @param  None
* @retval None
*/
void GPS_UART_RXCallBack(void)
{
  uint8_t GPS_Step = 0;
  uint8_t GPS_StepCount = 0;
  switch(GPS_UARTxM.GPSUart_State)
  {
  case GPS_UART_START:
    if( GPS_UARTxM.RecvByte == '$' )
    {
      GPS_UARTxM.GPSUart_State = GPS_UART_CMD_1;
    }
    break;
    
  case GPS_UART_CMD_1:
    if( GPS_UARTxM.RecvByte == 'G' )
    {
      GPS_UARTxM.GPSUart_State = GPS_UART_CMD_2;
    }
    else
      GPS_UARTxM.GPSUart_State = GPS_UART_START;
    break;
    
  case GPS_UART_CMD_2:
    if( GPS_UARTxM.RecvByte == 'P' )
    {
      GPS_UARTxM.GPSUart_State = GPS_UART_CMD_3;
    }
    else
      GPS_UARTxM.GPSUart_State = GPS_UART_START;
    break;
    
  case GPS_UART_CMD_3:
    if( GPS_UARTxM.RecvByte == 'G' )
    {
      GPS_UARTxM.GPSUart_State = GPS_UART_CMD_4;
    }
    else
      GPS_UARTxM.GPSUart_State = GPS_UART_START;
    break;
    
  case GPS_UART_CMD_4:
    if( GPS_UARTxM.RecvByte == 'G' )
    {
      GPS_UARTxM.GPSUart_State = GPS_UART_PROTOCOL_GGA;
    }
    else
      GPS_UARTxM.GPSUart_State = GPS_UART_START;
    break;
    
  case GPS_UART_PROTOCOL_GGA:
    if( GPS_UARTxM.RecvByte == 'A' )
    {
      GPS_UARTxM.GPSUart_Protocol = GPS_UARTxM.RecvByte;
      GPS_UARTxM.GPSUart_State = GPS_UART_END;
      memcpy(GPS_UARTxM.String.Buffer, "GPGGA", 5);
      GPS_UARTxM.String.iCnt = 5;
    }
    else
      GPS_UARTxM.GPSUart_State = GPS_UART_START;
    break;
    
  case GPS_UART_END:
    GPS_UARTxM.String.Buffer[GPS_UARTxM.String.iCnt++] = GPS_UARTxM.RecvByte;
    
    if( GPS_UARTxM.RecvByte == 0x0A )
      GPS_UARTxM.GPSUart_State = GPS_UART_CHECKSUM;
    else if(GPS_UARTxM.String.iCnt == (MAXBUFFSIZE_UART-1))
    {
      memset(GPS_UARTxM.String.Buffer,0,MAXBUFFSIZE_UART);
      GPS_UARTxM.GPSUart_State = GPS_UART_START;
    }
    
    break;
    
  case GPS_UART_CHECKSUM:
    if(CalcChecksum(GPS_UARTxM.String.Buffer, (GPS_UARTxM.String.iCnt-6)))
    {
      if( GPS_UARTxM.GPSUart_Protocol == 'A' )
      {
        memset((void *)&GPS_DATA,0,sizeof(GPS_DATA));
        for(uint8_t i=5; i<GPS_UARTxM.String.iCnt; i++)
        {
          if( GPS_UARTxM.String.Buffer[i] == ',' )
          {
            GPS_Step++;
            GPS_StepCount = 0;
          }
          else
          {
            switch(GPS_Step)
            {
            case 1:
              GPS_DATA.UTCTime[GPS_StepCount] = GPS_UARTxM.String.Buffer[i];
              break;
            case 2:
              GPS_DATA.Latitude[GPS_StepCount] = GPS_UARTxM.String.Buffer[i];
              GPS_DATA.Latitude_Count++;
              break;
            case 3:  
              GPS_DATA.Latitude_Direction = GPS_UARTxM.String.Buffer[i];
              break;  
            case 4:
              GPS_DATA.Longitude[GPS_StepCount] = GPS_UARTxM.String.Buffer[i];
              GPS_DATA.Longitude_Count++;
              break;
            case 5:  
              GPS_DATA.Longitude_Direction = GPS_UARTxM.String.Buffer[i];
              break;
            case 6:
              GPS_DATA.Position_Fix = GPS_UARTxM.String.Buffer[i];
              if( GPS_DATA.Position_Fix == 0x30 )
              {
                i = GPS_UARTxM.String.iCnt;
                memset((void *)&GPS_DATA,0,sizeof(GPS_DATA));
              }
              break;
            case 7:
              GPS_DATA.Satellites_Num[GPS_StepCount] = GPS_UARTxM.String.Buffer[i];
              break;
            case 8:
              i = GPS_UARTxM.String.iCnt;
              break;
            }
            GPS_StepCount++;
          }
          
        }
        
        if( GPS_DATA.Position_Fix >= 30 )
        {
          if( GPS_S.GPS_Set == 0 )
          {
            MX_RTC_PRINT(GPS_SET);
            GPS_S.GPS_Set = 1;
          }
        }
      }
    }
    memset(GPS_UARTxM.String.Buffer,0,MAXBUFFSIZE_UART);
    GPS_UARTxM.GPSUart_State = GPS_UART_START;
    break;
  }
}

/* USER CODE END 1 */

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
