/**
  ******************************************************************************
  * File Name          : GPS.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

#define USE_DEBUG_PRINT
/* GPS NMEA Protocol */
#define GPS_UART_NONE           0
#define GPS_UART_START          1
#define GPS_UART_CMD_1          2
#define GPS_UART_CMD_2          3
#define GPS_UART_CMD_3          4
#define GPS_UART_CMD_4          5
#define GPS_UART_PROTOCOL_GGA   6
#define GPS_UART_PROTOCOL_GSV   7
#define GPS_UART_END            8
#define GPS_UART_CHECKSUM       9

/* Summer Module's Command TX/RX */
#define SEPARATOR_CR                         (0x0d)
#define SEPARATOR_LF                         (0x0a)


/**
  * @brief  Typedef, GPS Data
  * @author shjung
  * @date   2017-11-23
**/
typedef struct { 
  uint8_t UTCTime[10];          //GPS UTC time buffer
  
  uint8_t Latitude[20];         //Latitude buffer
  uint8_t Latitude_Direction;   //Latitude direction
  uint8_t Latitude_Count;       //Latitude buffer count
  
  uint8_t Longitude[20];        //Longitude buffer
  uint8_t Longitude_Direction;  //Longitude direction
  uint8_t Longitude_Count;      //Longitude buffer count
  
  uint8_t Position_Fix;         //Latitude, Longitude fixed flag
  
  uint8_t Satellites_Num[2];    //GPS satellites Number
}_typeGPSData;

/**
  * @brief  Typedef, GPS UART State & Flags
  * @author shjung
  * @date   2017-11-23
**/
typedef struct {
  __IO ITStatus UartTxCplt;
  __IO ITStatus UartRxCplt;
  _typeString String;
  uint8_t RecvByte;             //GPS UART RX callback recvery 1byte
  uint8_t GPSUart_State;        //GPS NMEA Protocol state
  uint8_t GPSUart_Protocol;     //GPS NMEA protocol ('A')
}_typeGPS_UART;


/* USER CODE END Private defines */
extern _typeGPSData GPS_DATA;
extern _typeGPS_UART GPS_UARTxM;

void GPS_UART_RXCallBack(void);

/* USER CODE END Prototypes */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
