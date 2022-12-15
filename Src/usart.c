/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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


/* USER CODE BEGIN 0 */
#include <stdarg.h>
#include <string.h>
#include "usart.h"
#include "NBIoT.h"
#include "GPS.h"
#include "gpio.h"
#include "tim.h"
#include "rtc.h"

_typeUART Debug_UARTxM = 
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

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* Debug UART init function */
void MX_UART4_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* NBIot UART init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* GPS UART init function */
void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */
    
  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();
  
    /**UART4 GPIO Configuration    
    PA0     ------> UART4_TX
    PA1     ------> UART4_RX 
    */
    GPIO_InitStruct.Pin = DEBUG_UART_TX_Pin|DEBUG_UART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */
    
  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */
    
  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = NBIoT_UART_TX_Pin|NBIoT_UART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
    
  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */
    
  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = BLE_UART_TX_Pin|BLE_UART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */
    
  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */
    
  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PC4     ------> USART3_TX
    PC5     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPS_UART_TX_Pin|GPS_UART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */
    
  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */
    
  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PA0     ------> UART4_TX
    PA1     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOA, DEBUG_UART_TX_Pin|DEBUG_UART_RX_Pin);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */
    
  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */
    
  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOB, NBIoT_UART_TX_Pin|NBIoT_UART_RX_Pin);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */
    
  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */
    
  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, BLE_UART_TX_Pin|BLE_UART_RX_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */
    
  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */
    
  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PC4     ------> USART3_TX
    PC5     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPS_UART_TX_Pin|GPS_UART_RX_Pin);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */
    
  /* USER CODE END USART3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void USARTx_SendData(UART_HandleTypeDef *huart, char *str, uint16_t size) 
{
  uint16_t len = (size == NULL) ? strlen(str) : size;
  
  for(uint16_t i=0; i<len; i++) {
    huart->Instance->TDR = (str[i] & (uint16_t)0x01FF);
    while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) == RESET);
    }
}

bool Debug_UARTxM_RX_Start()
{
  bool bRes;
  /* Reset transmission flag */
  Debug_UARTxM.UartRxCplt = RESET;
  
  /*##-2- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive_IT(&huart4, (uint8_t *)&Debug_UARTxM.RecvByte, sizeof(uint8_t) ) != HAL_OK)
  {
    bRes = FALSE;
    _Error_Handler(__FILE__, __LINE__);
  }
  
  return bRes;
}

bool GPS_UARTxM_RX_Start()
{
  bool bRes;
  GPS_UARTxM.GPSUart_State = 1;
  /* Reset transmission flag */
  GPS_UARTxM.UartRxCplt = RESET;
  
  /*##-2- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive_IT(&huart3, (uint8_t *)&GPS_UARTxM.RecvByte, sizeof(uint8_t) ) != HAL_OK)
  {
    bRes = FALSE;
    _Error_Handler(__FILE__, __LINE__);
  }
  
  return bRes;
}

bool NBIoT_UARTxM_RX_Start()
{
  bool bRes;
  /* Reset transmission flag */
  NBIoT_UARTxM.UartRxCplt = RESET;
  
  /*##-2- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive_IT(&huart1, (uint8_t *)&NBIoT_UARTxM.RecvByte, sizeof(uint8_t) ) != HAL_OK)
  {
    bRes = FALSE;
    _Error_Handler(__FILE__, __LINE__);
  }
  
  return bRes;
}

/**
* @brief  UART RX set
* @param  none
* @retval none
*/
void uart_Config()
{
  NBIoT_UARTxM_RX_Start();  
  Debug_UARTxM_RX_Start();
  GPS_UARTxM_RX_Start();
}

/**
* @brief  Tx Transfer completed callback
* @param  UartHandle: UART handle. 
* @note   This example shows a simple way to report end of DMA Tx transfer, and 
*         you can add your own implementation. 
* @retval None
*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if(UartHandle->Instance == USART1)
  {
    /* Set transmission flag: trasfer complete*/
    NBIoT_UARTxM.UartTxCplt = SET; 
  }
}

/**
* @brief  Rx Receiver completed callback
* @param  UartHandle: UART handle. 
* @note   This example shows a simple way to report end of DMA Rx transfer, and 
*         you can add your own implementation. 
* @retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandleRx)
{
  /* UART Rx Callback : check to the Instance type */
  if(UartHandleRx->Instance == USART1)
  {
    /* Debug UART Print */
    HAL_UART_Transmit_IT(&huart4, &NBIoT_UARTxM.RecvByte, 1);
    /* NBIot UART RX callback */
    NBIoT_UART_RXCallBack();

    /* UART Rx Callback  : Anti Buffer-Overflow and UartRxCplt is SET */
    if( NBIoT_UARTxM.String.iCnt >= MAXBUFFSIZE_UART ) 
    {
      NBIoT_UARTxM.String.iCnt = 0;
      NBIoT_UARTxM.UartRxCplt = SET;
    }
    
    /* UART Rx Callback  : Enable UART 1 Byte Interrupt */
    if(HAL_UART_Receive_IT(&huart1, (uint8_t *)&NBIoT_UARTxM.RecvByte, sizeof(uint8_t) ) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
  }
  
  if(UartHandleRx->Instance == USART3)
  {
    /* GPS UART RX callback */
    GPS_UART_RXCallBack();
    
    /* UART Rx Callback : Anti Buffer-Overflow and UartRxCplt is SET */
    if( GPS_UARTxM.String.iCnt >= MAXBUFFSIZE_UART ) 
    {
      GPS_UARTxM.String.iCnt = 0;
      GPS_UARTxM.UartRxCplt = SET;
    }
    
    /* UART Rx Callback : Enable UART 1 Byte Interrupt */
    if(HAL_UART_Receive_IT(&huart3, (uint8_t *)&GPS_UARTxM.RecvByte, sizeof(uint8_t) ) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
  }
  
  
  if(UartHandleRx->Instance == UART4)
  {    
    /* UART Rx Callback : Anti Buffer-Overflow and UartRxCplt is SET */
    if( Debug_UARTxM.String.iCnt >= MAXBUFFSIZE_UART ) 
    {
      Debug_UARTxM.String.iCnt = 0;
      Debug_UARTxM.UartRxCplt = SET;
    }
    /* UART Rx Callback : Enable UART 1 Byte Interrupt */
    if(HAL_UART_Receive_IT(&huart4, (uint8_t *)&Debug_UARTxM.RecvByte, sizeof(uint8_t) ) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }
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
