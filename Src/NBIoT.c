/**
******************************************************************************
* File Name          : NBIoT.c
* Description        : This file provides code for the configuration
*                      of the NBIoT instances.
******************************************************************************
*/

/* USER CODE BEGIN 0 */
#include <stdarg.h>
#include <string.h>

#include "usart.h"
#include "gpio.h"
#include "NBIoT.h"
#include "GPS.h"
#include "tim.h"
#include "rtc.h"

uint16_t CAL_ASCII_TO_HEX(char *ascii, char *hex, uint16_t size_t);
uint8_t Checksum(char *data, uint8_t len);

HAL_StatusTypeDef NBIoT_CHECK_NETWORK(void);

static void CALC_CTN(char *rxIMSI);
static void CALC_MUICCID(char *rxMUICCID);
static void CALC_CGSN(char *rxCGSN);

/* Send Data Protocol info */

char GPS_NBIoTData[PROTOCOL_LEN]={0x30,};       //NBIot send GPS data buffer
char GPS_NBIoTData_Ascii[PROTOCOL_LEN*2]={0,};  //NBIot send GPS data Ascii buffer
uint32_t NBIoT_TX_Count = 0;                    //NBIot send GPS data count
uint8_t NBIoTCallback_buf[128];                 //NBIot UART RX callback buffer
uint8_t NBIot_init = 1;

/* NBIot DATA type */
_typeNBIoTData NBIoT_DATA = {0,};

extern uint8_t mainState;

_typeUART NBIoT_UARTxM = 
{
  RESET,
  RESET,
  .String = {
    0,
    0,
  },
  0,
};

enum{
  FALSE = 0,
  TRUE,
};

/* USER CODE END 0 */
extern UART_HandleTypeDef huart1;
/**
* @brief  NBIot Buffer clear
* @param  none
* @retval none
**/
void NBIoT_BUFFER_CLEAR(void)
{
  memset(NBIoT_DATA.NBIoT_CMD_RXBuf, 0, sizeof(NBIoT_DATA.NBIoT_CMD_RXBuf));
  NBIoT_DATA.NBIoT_CMD_RXCount = 0;
  NBIoT_DATA.UART_rxFlag = NO_RESPONSE;
  NBIoT_DATA.NBIoT_CMD_TXCommand_Flag = 0;
}

/**
* @brief  NBIot CALC CTN Init
* @param  NBIot IMSI UART data
* @retval none
**/
static void CALC_CTN(char *rxIMSI)
{
  NBIoT_DATA.Device_info.CTN_NUM[0] = 0xA1;       // Modify Cesco Request (17-11-15 by kyh)
  NBIoT_DATA.Device_info.CTN_NUM[1] = (NBIoT_DATA.NBIoT_info.imsi[1]<<4 & 0xF0) + (NBIoT_DATA.NBIoT_info.imsi[2] & 0x0F);
  NBIoT_DATA.Device_info.CTN_NUM[2] = (NBIoT_DATA.NBIoT_info.imsi[3]<<4 & 0xF0) + (NBIoT_DATA.NBIoT_info.imsi[4] & 0x0F);
  NBIoT_DATA.Device_info.CTN_NUM[3] = (NBIoT_DATA.NBIoT_info.imsi[5]<<4 & 0xF0) + (NBIoT_DATA.NBIoT_info.imsi[6] & 0x0F);
  NBIoT_DATA.Device_info.CTN_NUM[4] = (NBIoT_DATA.NBIoT_info.imsi[7]<<4 & 0xF0) + (NBIoT_DATA.NBIoT_info.imsi[8] & 0x0F);
  NBIoT_DATA.Device_info.CTN_NUM[5] = (NBIoT_DATA.NBIoT_info.imsi[9]<<4 & 0xF0) + (NBIoT_DATA.NBIoT_info.imsi[10] & 0x0F);
}

/**
* @brief  NBIot CALC CIMI Init
* @param  NBIot CIMI UART data
* @retval none
**/
static void CALC_CIMI(char *rxCIMI)
{
  char copyCIMI[50];
  char *correctStr = NULL;
  
  memset(copyCIMI, 0x00, sizeof(copyCIMI));
  correctStr = strstr(rxCIMI, "45006");
  
  memcpy(copyCIMI, correctStr+5, 10);
  sprintf(NBIoT_DATA.NBIoT_info.imsi, "0%s", copyCIMI);
  
  CALC_CTN(NBIoT_DATA.NBIoT_info.imsi);
  
}

/**
* @brief  NBIot CALC MUICCID Init
* @param  NBIot MUICCID UART data
* @retval none
**/
static void CALC_MUICCID(char *rxMUICCID)
{
  char copyMUICCID[50];
  char *correctStr = NULL;
  
  memset(copyMUICCID, 0x00, sizeof(copyMUICCID));
  correctStr = strstr(rxMUICCID, "+MUICCID:");
  
  memcpy(copyMUICCID, correctStr+9, 19);
  memcpy(NBIoT_DATA.NBIoT_info.iccid, copyMUICCID, strlen(copyMUICCID));
  
}

/**
* @brief  NBIot CALC CGSN Init
* @param  NBIot CGSN UART data
* @retval none
**/
static void CALC_CGSN(char *rxCGSN)
{
  char copyCGSN[50];
  char *correctStr = NULL;
  
  memset(copyCGSN, 0x00, sizeof(copyCGSN));
  correctStr = strstr(rxCGSN, "+CGSN:");
  
  memcpy(copyCGSN, correctStr+6, 15);
  memcpy(NBIoT_DATA.NBIoT_info.imei, copyCGSN, strlen(copyCGSN));
  
}

/**
* @brief  NBIot UDP IP, Port Init
* @param  none
* @retval none
**/
void NBIoT_UDP_Init(void)
{
  NBIoT_DATA.NBIoT_CMD_IPReset_Flag = 0; 
  
  {
    NBIoT_DATA.UDP_info.server_udp_ip     = ((uint32_t)UDP_ADDR0<<24)+((uint32_t)UDP_ADDR1<<16)+((uint32_t)UDP_ADDR2<<8)+((uint32_t)UDP_ADDR3);
    NBIoT_DATA.UDP_info.server_udp_port   = ((uint32_t)UDP_PORT);
    
    sprintf(NBIoT_DATA.UDP_IP_CkBuf,"0,%d.%d.%d.%d,%d,28,3", 
            0xFF&((uint32_t)NBIoT_DATA.UDP_info.server_udp_ip>>24),
            0xFF&((uint32_t)NBIoT_DATA.UDP_info.server_udp_ip>>16),
            0xFF&((uint32_t)NBIoT_DATA.UDP_info.server_udp_ip>>8),
            0xFF&((uint32_t)NBIoT_DATA.UDP_info.server_udp_ip),
            NBIoT_DATA.UDP_info.server_udp_port
              );
  }
}

/**
* @brief  NBIot Command pass timeout (1bit -> 1ms)
* @param  Timeout value
* @retval HAL status
**/
HAL_StatusTypeDef NBIoT_TIMEOUT(uint32_t timeout)
{
  uint32_t firstTick = HAL_GetTick();
  while( (HAL_GetTick()-firstTick) < timeout )
  {    
    if(NBIoT_DATA.UART_rxFlag == OK)
      return HAL_OK;
    else if( NBIoT_DATA.UART_rxFlag == NETWORK)
      return HAL_OK;
    else if( NBIoT_DATA.UART_rxFlag == BOOT )
      return HAL_OK;
    else if(NBIoT_DATA.UART_rxFlag == ERROR)
      return HAL_ERROR;
    else if(NBIoT_DATA.UART_rxFlag == RF_RECV)
      return HAL_OK;
    else if(NBIoT_DATA.UART_rxFlag == RX_BUFCK)
      return HAL_OK;
  }
  return HAL_TIMEOUT;
}

/**
* @brief  NBIot UART Send command
* @param  NBIot AT Command, Command timeout
* @retval HAL status
**/
HAL_StatusTypeDef NBIoT_SEND_CMD(char *command, uint32_t timeout)
{
  /*  Initialize UART2 TX&RX Buffer */
  NBIoT_BUFFER_CLEAR();
  
  HAL_StatusTypeDef err = HAL_ERROR;
  uint8_t retryCount = 0;
  uint16_t TxLen = 0;
  
  do{
    TxLen = sprintf(NBIoT_DATA.NBIoT_CMD_TXBuf, command);
#ifdef USE_DEBUG_PRINT
    USARTx_SendData(&huart4, NBIoT_DATA.NBIoT_CMD_TXBuf, TxLen);
    HAL_Delay(1);
#endif
    USARTx_SendData(&huart1, NBIoT_DATA.NBIoT_CMD_TXBuf, TxLen);
    /*  Check TX Message(Wait for Delay & NO ACK) */
    if(strstr(command, "AT+NRB\r\n")!=NULL)
    {
      return HAL_BUSY;
    }
    else if(strstr(command, "AT+MLWULDATA")!=NULL)
    {
      return HAL_OK;
    }
    else if(strstr(command, "AT+CEREG")!=NULL)
    {
      return HAL_OK;
    }
    
    if(strstr(command, "AT+CGDCONT")!=NULL)
    {
      NBIoT_DATA.NBIoT_CMD_TXCommand_Flag = 1;
    }
    else if(strstr(command, "AT+COPS")!=NULL)
    {
      NBIoT_DATA.NBIoT_CMD_TXCommand_Flag = 1;
    }

    /*  Wait for RX Message */
    err = NBIoT_TIMEOUT(timeout);
    
    /*  Check & Process RX Message */
#ifdef USE_DEBUG_PRINT
    if(err == HAL_ERROR)
      USARTx_SendData(&huart4, "RX:ERROR\r\n", NULL);
    else if(err == HAL_TIMEOUT)
      USARTx_SendData(&huart4, "RX:TIMEOUT\r\n", NULL);
#endif
    if(err == HAL_OK)
    {
      if(strstr(command, "AT+CIMI\r\n")!=NULL)
      {
        CALC_CIMI(NBIoT_DATA.NBIoT_CMD_RXBuf);
      }
      else if(strstr(command, "AT+MUICCID\r\n")!=NULL)
      {
        CALC_MUICCID(NBIoT_DATA.NBIoT_CMD_RXBuf);
      }
      else if(strstr(command, "AT+CGSN=1\r\n")!=NULL)
      {
        CALC_CGSN(NBIoT_DATA.NBIoT_CMD_RXBuf);
      }
      else if(strstr(command, "AT+NSORF")!=NULL) //UDP Appliaction ack
      {
        RTC_UPDATE(NBIoT_DATA.UDP_Payload_Buf); //Server RTC Time update
        memset(NBIoT_DATA.UDP_Payload_Buf,0,sizeof(NBIoT_DATA.UDP_Payload_Buf));
        NBIoT_DATA.TxFail_count = 0;
        NBIoT_DATA.initial_state++;
      }
      /*  Initialize UART2 TX&RX Buffer */
      NBIoT_BUFFER_CLEAR();
      
      return HAL_OK;
    }
    else if(err == HAL_ERROR)
    {
      if(strstr(command, "AT+NSOST=") != NULL)
      {
        USARTx_SendData(&huart4, "UDP_SEND_Failed\r\n", 17);
        NBIoT_DATA.initial_state = 10;
      }
      else if(strstr(command, "AT+NSORF")!=NULL)
      {
        USARTx_SendData(&huart4, "UDP_ACK_Failed\r\n", 16);
        NBIoT_DATA.initial_state = 10;
      }
      /*  Initialize UART2 TX&RX Buffer */
      NBIoT_BUFFER_CLEAR();
      
      return HAL_ERROR;
    }
    else
    {
      if(strstr(command, "AT+NSOST=") != NULL)
      {
        USARTx_SendData(&huart4, "UDP_SEND_Failed\r\n", 17);
        NBIoT_DATA.initial_state = 10;
      }
      else if(strstr(command, "AT+NSORF")!=NULL)
      {
        USARTx_SendData(&huart4, "UDP_ACK_Failed\r\n", 16);
        NBIoT_DATA.initial_state = 10;
      }
      /*  Initialize UART2 TX&RX Buffer */
      NBIoT_BUFFER_CLEAR();
      
      retryCount++;
    }
  }while( retryCount < MAX_SUMMER_RETRY_COUNT); // Max retry count (2)
  
  return HAL_TIMEOUT;
}

/**
* @brief  NBIot Network check
* @param  none
* @retval HAL status
**/
HAL_StatusTypeDef NBIoT_CHECK_NETWORK(void)
{
  char celltower[8] = {0,};
  
  if(NBIoT_DATA.UART_rxFlag >= NETWORK)
  {
    //UART RX message '+CEREG:1', Network join success
    memcpy(NBIoT_DATA.NBIoT_CMD_RXBuf, strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "+CEREG:1"), sizeof(NBIoT_DATA.NBIoT_CMD_RXBuf));
    memcpy(celltower, &NBIoT_DATA.NBIoT_CMD_RXBuf[13], sizeof(celltower));
    celltower[0] = '0';    // ',' -> '0'
    
#ifdef USE_DEBUG_PRINT
    USARTx_SendData(&huart4, "Cell Tower = ", strlen("Cell Tower = "));
    USARTx_SendData(&huart4, celltower, 8);
#endif
    return HAL_OK;
  }
  return HAL_ERROR;
}

/**
* @brief  Ascii to hex
* @param  Ascii buffer, Return hex buffer, Ascii size
* @retval hex size
**/
uint16_t CAL_ASCII_TO_HEX(char *ascii, char *hex, uint16_t size_t)
{
  uint16_t size = size_t;
  char temp[1]={NULL};
  
  if(size != 0)
  {
    for(uint16_t i=0; i<size; i++)
    {
      sprintf(temp, "%02X", ascii[i]);
      strcat(hex, temp);
      temp[0] = NULL;
    }
  }
  
  return size;
}

/**
* @brief  NBIot UDP Transmit Optimize
* @param  none
* @retval HAL status
**/
HAL_StatusTypeDef NBIoT_UDP_SOCKET_TRANSMIT_Optimize()
{
  // char *msg -> ordinary usart1's TX Buffer use
  uint16_t array_size = 0;  
  
//  memset(GPS_NBIoTData, 0x30, PROTOCOL_LEN);
  memset(GPS_NBIoTData_Ascii, 0, PROTOCOL_LEN*2);
  /*
  GPS_DATA.Latitude[] 
  GPS_DATA.Latitude_Count
  GPS_DATA.Latitude_Direction 
  GPS_DATA.Longitude[]
  GPS_DATA.Longitude_Count
  GPS_DATA.Longitude_Direction
  */
  {
    NBIoT_TX_Count++;
    
    GPS_NBIoTData[0] = ((NBIoT_TX_Count/1000)%10)+0x30;
    GPS_NBIoTData[1] = ((NBIoT_TX_Count/100)%10)+0x30;
    GPS_NBIoTData[2] = ((NBIoT_TX_Count/10)%10)+0x30;
    GPS_NBIoTData[3] = (NBIoT_TX_Count%10)+0x30;
    
    memcpy(&GPS_NBIoTData[4], NBIoT_DATA.NBIoT_info.imsi, 11);
    
    GPS_NBIoTData[39] = Checksum(GPS_NBIoTData, 39);
    
    array_size = CAL_ASCII_TO_HEX(GPS_NBIoTData, GPS_NBIoTData_Ascii, PROTOCOL_LEN);
  }
  __no_operation();
  
  memset(NBIoT_DATA.NBIoT_CMD_TXBuf, 0x00, sizeof(NBIoT_DATA.NBIoT_CMD_TXBuf));   // Optimize Routine
  sprintf(NBIoT_DATA.NBIoT_CMD_TXBuf, "AT+NSOST=0,%d.%d.%d.%d,%d,%d,%s\r\n", 
          0xFF&((uint32_t)NBIoT_DATA.UDP_info.server_udp_ip>>24),
          0xFF&((uint32_t)NBIoT_DATA.UDP_info.server_udp_ip>>16),
          0xFF&((uint32_t)NBIoT_DATA.UDP_info.server_udp_ip>>8),
          0xFF&((uint32_t)NBIoT_DATA.UDP_info.server_udp_ip),
          NBIoT_DATA.UDP_info.server_udp_port,
          array_size,GPS_NBIoTData_Ascii);
  
  
  NBIoT_SEND_CMD(NBIoT_DATA.NBIoT_CMD_TXBuf, MAX_SUMMER_USART_TIMEOUT);
  
  memset((void *)&GPS_DATA, 0, sizeof(GPS_DATA));
  return HAL_OK;
}

/**
* @brief  RTC Wakeup, NBIot state init
* @param  none
* @retval none
**/
void NBIoT_RTCWakeup_State(void)
{
  NBIoT_BUFFER_CLEAR();
  if(NBIoT_DATA.initial_state > 14)
  {
    NBIoT_DATA.initial_state = 14;
  }
  else
  {
    if( NBIoT_DATA.initial_state != 0 )
      NBIoT_DATA.initial_state = 10;
  }
}

/**
* @brief  NBIot main process
* @param  none
* @retval none
**/
void NBIoT_Init_Process(void)
{
  int printf_len = 0;
  switch(NBIoT_DATA.initial_state)
  {
  case 0:
    NBIoT_BUFFER_CLEAR();
    NBIoT_DATA.initial_state++;
    break;
    
  case 1:
    //NBIoT_SEND_CMD("AT+CPSMS=1\r\n", MAX_SUMMER_USART_TIMEOUT);
    NBIoT_SEND_CMD("AT+CGMI\r\n", MAX_SUMMER_USART_TIMEOUT);
    NBIoT_DATA.initial_state++;
    break;
    
  case 2:
    NBIoT_DATA.initial_state++;
    break;
    
  case 3:
    NBIoT_SEND_CMD("AT+CIMI\r\n",   MAX_SUMMER_USART_TIMEOUT);
    NBIoT_DATA.initial_state++;
    break;
    
  case 4:
    NBIoT_SEND_CMD("AT+MUICCID\r\n",MAX_SUMMER_USART_TIMEOUT);
    NBIoT_DATA.initial_state++;
    break;
    
  case 5:
    NBIoT_SEND_CMD("AT+CGSN=1\r\n", MAX_SUMMER_USART_TIMEOUT);
    NBIoT_DATA.initial_state++;
    break;
    
  case 6:
    NBIoT_SEND_CMD("AT+CFUN=0\r\n", 1000*8);
    NBIoT_DATA.initial_state++;
    break;
    
  case 7:
    NBIoT_SEND_CMD("AT+NBAND=5\r\n",MAX_SUMMER_USART_TIMEOUT);
    NBIoT_DATA.initial_state++;
    break;
    
  case 8:
    NBIoT_DATA.initial_state++;
    break;
    
  case 9:
    NBIoT_DATA.initial_state++;
    break;
    
  case 10:
    /* NBIot Reset command */
    NBIoT_SEND_CMD("AT+NRB\r\n", 1000*30);
    NBIoT_TIMEOUT(1000*10);
    NBIoT_DATA.initial_state++;
    break;
    
  case 11:
    /* NBIot Network join command */
    NBIoT_SEND_CMD("AT+CEREG=2\r\n", 1000*30);
    NBIoT_TIMEOUT(1000*60); //30s -> 60s (181107)
    
    if( NBIoT_CHECK_NETWORK() == HAL_OK )
      NBIoT_DATA.initial_state++;
    else
    {
      /* Network join fail, HW Reset 181107  */
      NBIoT_PowerOff();
      HAL_Delay(500);
      NBIoT_PowerOn();
      NBIoT_DATA.initial_state = 0;
            
      NBIoT_DATA.CEREG_ErrorCount++;
      memset(NBIoT_DATA.NBIoT_CMD_TXBuf, 0x00, sizeof(NBIoT_DATA.NBIoT_CMD_TXBuf));
      printf_len = sprintf(NBIoT_DATA.NBIoT_CMD_TXBuf, "CEREG_Error : %d\r\n", NBIoT_DATA.CEREG_ErrorCount);
      USARTx_SendData(&huart4, NBIoT_DATA.NBIoT_CMD_TXBuf, printf_len);
      
      NBIoT_TIMEOUT(1000*60);//HW Reset 60s Timeout (181107)
      
      if( NBIoT_DATA.CEREG_ErrorCount >= 3 ) //3 -> 3 (181107)
      {
        NBIoT_DATA.CEREG_ErrorCount = 0;
        mainState = SLEEP_STATE;
        NBIoT_DATA.initial_state = 0;
      }
    }
    break;
    
  case 12:
    NBIoT_DATA.initial_state++;
    break;
    
  case 13:
    /* NBIot UDP ip, port set command */
    memset(NBIoT_DATA.NBIoT_CMD_TXBuf, 0x00, sizeof(NBIoT_DATA.NBIoT_CMD_TXBuf));
    printf_len = sprintf(NBIoT_DATA.NBIoT_CMD_TXBuf, "AT+NSOCR=DGRAM,17,%d,1\r\n", NBIoT_DATA.UDP_info.server_udp_port);
    NBIoT_SEND_CMD(NBIoT_DATA.NBIoT_CMD_TXBuf,MAX_SUMMER_USART_TIMEOUT);
    MX_RTC_PRINT(NBIoT_SET);
    NBIoT_DATA.initial_state++;
    break;
    
  case 14:
    /* NBIot UDP Transmit command */
    NBIoT_UDP_SOCKET_TRANSMIT_Optimize();
    NBIoT_TIMEOUT(1000*60); // Ack timeout 
    
    /* NBIot UDP Ack response check */
    if( NBIoT_DATA.UART_rxFlag == RF_RECV )
    {
      uint8_t i=0;
      uint8_t sockNum = 0;
      uint16_t datalen = 0;
      char *parseArray[6] = {0x00,};
      char *ptr;
      
      ptr = strtok(NBIoT_DATA.NBIoT_CMD_RXBuf, ":,\r\n");
      parseArray[i++] = ptr;
      
      while(ptr != NULL)
      {
        ptr = strtok(NULL, ",\r\n");
        parseArray[i++] = ptr;
      }
      
      if(parseArray != NULL)
      {
        if( i >= 3 )
        {
          sockNum = parseArray[1][0]-'0';
          
          if(strlen(parseArray[2]) == 1)
            datalen = (parseArray[2][0]-'0')*1;
          else if(strlen(parseArray[2]) == 2)
            datalen = (parseArray[2][0]-'0')*10 + (parseArray[2][1]-'0')*1;
          else if(strlen(parseArray[2]) == 3)
            datalen = (parseArray[2][0]-'0')*100 + (parseArray[2][1]*10-'0') + (parseArray[2][2]-'0')*1;
          
          memset(NBIoT_DATA.NBIoT_CMD_TXBuf, 0x00, sizeof(NBIoT_DATA.NBIoT_CMD_TXBuf));
          sprintf(NBIoT_DATA.NBIoT_CMD_TXBuf, "AT+NSORF=%d,%d\r\n", sockNum, datalen);      // sockNum
          NBIoT_DATA.UDP_ACK_Flag = 1;
          /* NBIot UDP Ack messages read command */
          NBIoT_SEND_CMD(NBIoT_DATA.NBIoT_CMD_TXBuf, 1000*10);
        }
      }
    }
    else
    {
      NBIoT_DATA.initial_state = 10;
    }
    
    /* NBIot UDP Transmit retry count */
    NBIoT_DATA.TxFail_count++;
    if( NBIoT_DATA.TxFail_count >= 3 )
    {
      NBIoT_DATA.TxFail_count = 0;
      mainState = SLEEP_STATE;
    }
    break;
    
  case 15:
    NBIoT_DATA.initial_state++;
    break;
    
  case 16:
    /* MAC Sleep */
    NBIoT_DATA.TxFail_count = 0;
    mainState = SLEEP_STATE;
    break;
  }
  
}

void NBIoT_GPSData_Save(void)
{
  GPS_PowerOff();
  mainState = NBIOT_STATE;
  memset(GPS_NBIoTData, 0x30, PROTOCOL_LEN);
  
  uint8_t GPS_DATA_Count = LATITUDE_START_LEN;
  if( GPS_DATA.Latitude_Direction  == 'N' )
  {
    GPS_NBIoTData[GPS_DATA_Count++] = '+';
    memcpy(&GPS_NBIoTData[GPS_DATA_Count], GPS_DATA.Latitude, GPS_DATA.Latitude_Count );
  }
  else
  {
    GPS_NBIoTData[GPS_DATA_Count++] = '-';
    memcpy(&GPS_NBIoTData[GPS_DATA_Count], GPS_DATA.Latitude, GPS_DATA.Latitude_Count );
  }
  
  GPS_DATA_Count = LONGITUDE_START_LEN;
  
  if( GPS_DATA.Longitude_Direction  == 'E' )
  {
    GPS_NBIoTData[GPS_DATA_Count++] = '+';
    memcpy(&GPS_NBIoTData[GPS_DATA_Count], GPS_DATA.Longitude, GPS_DATA.Longitude_Count );
  }
  else
  {
    GPS_NBIoTData[GPS_DATA_Count++] = '-';
    memcpy(&GPS_NBIoTData[GPS_DATA_Count], GPS_DATA.Longitude, GPS_DATA.Longitude_Count );
  }
}
/**
* @brief  NBIot UDP Protocol checksum
* @param  NBIot UDP Protocol data, NBIot UDP Protocol length
* @retval checksum
**/
uint8_t Checksum(char *data, uint8_t len)
{
  char sum = 0;
  
  sum = data[0];
  for(int i = 1; i <= len; i++) {
    sum = sum ^ data[i];
  }
  
  return sum;
}

/**
* @brief  NBIot UART RX Callback
* @param  None
* @retval None
*/
void NBIoT_UART_RXCallBack(void)
{
  if(NBIoT_DATA.UART_rxFlag == NO_RESPONSE)
  {
    
    if( (strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "OK\r") != NULL) && (NBIot_init == 1) )
    {
      NBIot_init = 0;
      NBIoT_DATA.UART_rxFlag = OK;
    }
    
    NBIoT_DATA.NBIoT_CMD_RXBuf[NBIoT_DATA.NBIoT_CMD_RXCount++] = NBIoT_UARTxM.RecvByte;
    // BOOT Message
    // REBOOT_CAUSE_APPLICATION_WATCHDOG
    if( strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "REBOOT_CAUSE_SECURITY_RESET_UNKNOWN\r\n\r\nNeul \r\n\r\n")!=NULL ||
       strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "REBOOT_CAUSE_APPLICATION_AT\r\n\r\nNeul \r\n\r\nOK\r\n\r\n")!=NULL ||
         strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "REBOOT_CAUSE_UNKNOWN\r\n\r\nNeul \r\n\r\n")!=NULL )
    {
      NBIoT_DATA.UART_rxFlag = BOOT;
    }
    // OK
    else if((NBIoT_DATA.NBIoT_CMD_RXBuf[NBIoT_DATA.NBIoT_CMD_RXCount-1] == SEPARATOR_LF) && 
            (NBIoT_DATA.NBIoT_CMD_RXBuf[NBIoT_DATA.NBIoT_CMD_RXCount-2] == SEPARATOR_CR) &&
              (NBIoT_DATA.NBIoT_CMD_RXCount >= 8) )
    {
      if( (strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "AT+MLWEVTIND=")!=NULL)  ||
         (strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "+CEREG:") != NULL )     )
      {
        memcpy(NBIoTCallback_buf, strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "+CEREG:"), sizeof(NBIoTCallback_buf));
        if( NBIoTCallback_buf[7] == '1' )
          NBIoT_DATA.UART_rxFlag = NETWORK;
        else
          NBIoT_BUFFER_CLEAR();
      }
      else if( strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "+NSONMI:")!=NULL  )
        NBIoT_DATA.UART_rxFlag = RF_RECV;
      else if( strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "\r\nOK\r\n") != NULL )
      {
        if( (NBIoT_DATA.initial_state == 10) || (NBIoT_DATA.initial_state == 11) )
          NBIoT_DATA.UART_rxFlag = NO_RESPONSE;
        else
          NBIoT_DATA.UART_rxFlag = OK;
        
      }
      
    }
    else if( strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "45006")!=NULL &&
            strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "OK\r\n\r\n")!=NULL )
    {
      NBIoT_DATA.UART_rxFlag = OK;
    }
    // ERROR
    else if(strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "ERROR\r\n")!=NULL)
    {
      NBIoT_DATA.UART_rxFlag = ERROR;
    }
    else if(strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "OK\r\n")!=NULL )
    {
      if( (NBIoT_DATA.initial_state == 10) || (NBIoT_DATA.initial_state == 11) )
        NBIoT_DATA.UART_rxFlag = NO_RESPONSE;
      else
        NBIoT_DATA.UART_rxFlag = OK;
    }
    else if(strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, "OK")!=NULL && 
            NBIoT_DATA.NBIoT_CMD_TXCommand_Flag == 1)
    {
      NBIoT_DATA.UART_rxFlag = OK;
      NBIoT_DATA.NBIoT_CMD_TXCommand_Flag = 0;
    }
    else if( NBIoT_DATA.UDP_ACK_Flag == 1 )
    {
      if( strstr(NBIoT_DATA.NBIoT_CMD_RXBuf, NBIoT_DATA.UDP_IP_CkBuf)!=NULL )
      {
        if( NBIoT_DATA.UDP_ACK_Count < 56 )
          NBIoT_DATA.UDP_Payload_Buf[NBIoT_DATA.UDP_ACK_Count++] = NBIoT_UARTxM.RecvByte;
        else
        {
          NBIoT_DATA.UDP_ACK_Flag = 0;
          NBIoT_DATA.UDP_ACK_Count = 0;
          
          NBIoT_DATA.UART_rxFlag = RX_BUFCK;
        }
      }
    }
  }
}

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
