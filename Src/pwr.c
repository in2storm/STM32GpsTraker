/**
******************************************************************************
* @file    \Drivers\Intsain\Violet_Branch\Src\Violet_Branch_Driver.c
* @author  bglee, H/W 1 Team, Laboratory in Intsain 
* @version V1.0.0
* @date    24-Nov-2016
******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "pwr.h"
#include "gpio.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t enterPowerDown(_POWERDOWN_Type pwdMode);
void SleepMode_Measure(void);
void WakeupMode_Measure(void);
/* Private variables ---------------------------------------------------------*/
_POWER POWER =
{
  enterPowerDown,
};
/* RTC handler declaration */
RTC_HandleTypeDef RTCHandle;
/* Private functions ---------------------------------------------------------*/
uint8_t enterPowerDown(_POWERDOWN_Type pwdMode)
{
  uint8_t iRes = 0;
  
  /* Enable Power Clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  switch(pwdMode)
  {
  case sleepMode:
    SleepMode_Measure();
    break;
  case wakeupMode:
    WakeupMode_Measure();
    break;

  }
  
  return iRes;
}

/**
* @brief  This function configures the system to enter Stop mode with RTC 
*         clocked by LSE or LSI for current consumption measurement purpose.
*         STOP Mode with RTC clocked by LSE/LSI
*         =====================================   
*           - RTC Clocked by LSE or LSI
*           - Regulator in LP mode
*           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
*           - No IWDG
*           - Wakeup using EXTI Line (User push-button PA.00)
* @param  None
* @retval None
*/
void SleepMode_Measure(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
  /* Enable GPIOs clock */
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
#ifdef DEBUG
  //Not Used, PC13(MCU S/W)
  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_13;
//  GPIO_InitStruct.Pin = GPIO_PIN_All;
#endif
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
#ifdef DEBUG
  GPIO_InitStruct.Pin = GPIO_PIN_All;
#endif  
  //All pin Setup to Pull-down
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
#ifdef DEBUG
    GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_13 & ~GPIO_PIN_14;
//    GPIO_InitStruct.Pin = GPIO_PIN_All;
#endif 
  //All pin Setup to Pull-down
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);   
  
  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  
    /*Suspend Tick increment to prevent wakeup by Systick interrupt. 
  Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
  HAL_SuspendTick();
  
  /* Enter Stop Mode */
//  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
  /* Resume Tick interrupt if disabled prior to sleep mode entry*/
  HAL_ResumeTick();
    
  /* Configures system clock after wake-up from STOP: enable HSI and PLL with HSI as source*/
  SystemClock_Config();
}

/**
* @brief  This function configures the system to enter Stop mode with RTC 
*         clocked by LSE or LSI for current consumption measurement purpose.
*         STOP Mode with RTC clocked by LSE/LSI
*         =====================================   
*           - RTC Clocked by LSE or LSI
*           - Regulator in LP mode
*           - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
*           - No IWDG
*           - Wakeup using EXTI Line (User push-button PA.00)
* @param  None
* @retval None
*/
void WakeupMode_Measure(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
  /* Enable GPIOs clock */
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
    
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
#ifdef DEBUG
  //Excepted to Wake-up Pins, PC13(MCU S/W)
  GPIO_InitStruct.Pin = GPIO_PIN_All;// & ~GPIO_PIN_13;
  //#elif RELEASE
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_13;
#endif
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
#ifdef DEBUG
  //Excepted to Wakeup Pins, PB0 & PB2, Debug Pins
  //Excepted to Debug Pins, PB9(TCXO PIN)
//  GPIO_InitStruct.Pin = GPIO_PIN_All & ~PT400XXSR_WAKEUP_MODEM_PIN & ~GPIO_PIN_0  & ~GPIO_PIN_9 & ~PT400XXSR_WAKEUP_EXT_PIN;//;
  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_9;
#endif  
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  //  
  //#ifdef DEBUG
  //  //Excepted to Wake-up Pins, PC13(MCU S/W)
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_13;
  //#elif RELEASE
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_13;
  //#endif
  //  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  //  
  //#ifdef DEBUG
  //  //Excepted to Wakeup Pins, PB0 & PB2, Debug Pins
  //  //Excepted to Debug Pins, PB9(TCXO PIN)
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_0 & ~GPIO_PIN_9 & ~PT400XXSR_WAKEUP_EXT_PIN & ~PT400XXSR_WAKEUP_MODEM_PIN;
  //#elif RELEASE
  //  //Excepted to Wakeup Pins, PB0 & PB2,
  //  //Excepted to Debug Pins, PB9(TCXO PIN)
  //  //Excepted to Wakeup Pins, PB14(Wake-Up External PIN)
  //  //Excepted to Wakeup Pins, PB15(Wake-Up Modem PIN)
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_0 & ~GPIO_PIN_9 & ~PT400XXSR_WAKEUP_EXT_PIN & ~PT400XXSR_WAKEUP_MODEM_PIN;
  //#endif  
  //  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  //  
  //#ifdef DEBUG
  //  //Excepted to Wakeup Pins 0, 2, PA13 & PA14, Debug Pins
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_0 & ~GPIO_PIN_13 & ~GPIO_PIN_14;
  //#else
  //  //Wakeup Pins 0, 2
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & ~GPIO_PIN_0;
  //#endif
  //  
  //  
  //#ifdef DEBUG
  //  //Excepted to SWDIO Pins PA13 & PA14
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & PT400XXSR_RESET_PIN & ~GPIO_PIN_13 & ~GPIO_PIN_14;
  //#elif RELEASE
  //  GPIO_InitStruct.Pin = GPIO_PIN_All & PT400XXSR_RESET_PIN;
  //#endif  
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
  
  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  
  /* Enter Stop Mode */
  //  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
//  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
  /* Configures system clock after wake-up from STOP: enable HSI and PLL with HSI as source*/
//  SystemClock_Config(HSE_RESTORE);
}
