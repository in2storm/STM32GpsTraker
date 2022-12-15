/**
  ******************************************************************************
  * File Name          : NBIoT.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */
   

/* USER CODE BEGIN Private defines */

#define USE_DEBUG_PRINT

/* Summer Module's Command TX/RX */
#define SEPARATOR_CR                         (0x0d)
#define SEPARATOR_LF                         (0x0a)

#define NO_RESPONSE             (0)
#define ERROR                   (1)
#define OK                      (2)
#define BOOT                    (3)
#define NETWORK                 (4)
#define RF_RECV                 (5)
#define RX_BUFCK                (6)

#define UDP_ADDR0   (121)
#define UDP_ADDR1   (162)
#define UDP_ADDR2   (60)
#define UDP_ADDR3   (159)
#define UDP_PORT    (8888)

#define MAX_SUMMER_RETRY_COUNT          (2)
#define MAX_SUMMER_RESET_TIMEOUT        (1000*10)
#define MAX_SUMMER_USART_TIMEOUT        (1000*10)
  
#define PROTOCOL_LEN 40
#define LATITUDE_START_LEN 15
#define LONGITUDE_START_LEN 25

extern UART_HandleTypeDef huart1;

/**
  * @brief  Typedef, NBIoT Data
  * @author shjung
  * @date   2017-11-23
**/
typedef struct {
  char imsi[50];        // 01222992008
  char iccid[50];       // 8982068597009698874
  char imei[50];        // 861921031211977
} NBIOT_INFO_BUFFER;

typedef struct {
  uint8_t CTN_NUM[6];   // 012-1234-5678  >> 0x0012 1234 5678
  uint8_t RTC_TIME[5];          
} DEVICE_INFO_BUFFER;

typedef struct {
  uint32_t server_udp_ip;       // UDP Server IP
  uint32_t server_udp_port;     // UDP Server PortNumber
} UDP_INFO_BUFFER;

typedef struct { 
  char                  NBIoT_CMD_TXBuf[MAXBUFFSIZE_UART];      //NBIot UART TX buffer
  char                  NBIoT_CMD_RXBuf[MAXBUFFSIZE_UART];      //NBIot UART RX buffer
  uint16_t              NBIoT_CMD_RXCount;                      //NBIot RX Count
  uint8_t               NBIoT_CMD_TXCommand_Flag;               //NBIot TX OK command flag
  
  char                  UDP_IP_CkBuf[MAXBUFFSIZE_UART];         //NBIot UDP IP check buffer
  char                  UDP_Payload_Buf[56];                    //NBIot UDP ACK payload buffer
  uint8_t               UDP_ACK_Flag;                           //NBIot UDP ACK response flag
  uint8_t               UDP_ACK_Count;                          //NBIot UDP ACK response count
  
  uint8_t               NBIoT_CMD_IPReset_Flag;                 //NBIot IP Reset command flag
  
  DEVICE_INFO_BUFFER    Device_info;                            //NBIot device information
  NBIOT_INFO_BUFFER     NBIoT_info;                             //NBIot information
  UDP_INFO_BUFFER       UDP_info;                               //NBIot UDP information
  
  uint8_t               initial_state;                          //NBIot main state
  uint8_t               TxFail_count;                           //NBIot send data retry count
  uint8_t               UART_rxFlag;                            //NBIot UART RX flag
  uint8_t               CEREG_ErrorCount;                       //NBIot CEREG Command retry count
}_typeNBIoTData;


/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
extern _typeUART NBIoT_UARTxM;
extern char GPS_NBIoTData[PROTOCOL_LEN];

HAL_StatusTypeDef  NBIoT_SEND_CMD(char *command, uint32_t timeout);
HAL_StatusTypeDef NBIoT_TIMEOUT(uint32_t timeout);
void NBIoT_UART_RXCallBack(void);
void NBIoT_RTCWakeup_State(void);
void NBIoT_Init_Process(void);
void NBIoT_BUFFER_CLEAR(void);
void NBIoT_UDP_Init(void);
void NBIoT_GPSData_Save(void);

/* USER CODE END Prototypes */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
