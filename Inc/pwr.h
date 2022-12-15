/**
  ******************************************************************************
  * @file    \Drivers\Intsain\Violet-Branch\Inc\Violet-Branch_PWR.h
  * @author  bglee, H/W 1 Team, Laboratory in Intsain 
  * @version V1.0.0
  * @date    05-April-2017
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VIOLET_BRANCH_PWR_H
#define __VIOLET_BRANCH_PWR_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum{
  sleepMode = 0,
  wakeupMode,
}_POWERDOWN_Type;
typedef struct {
  uint8_t (*enterPowerDown)(_POWERDOWN_Type);
}_POWER;
extern _POWER POWER;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define RTC_CLOCK_SOURCE_LSE    /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */   /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */
#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV    0x7F
#define RTC_SYNCH_PREDIV     0x0130
#endif
#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif

/* Exported functions ------------------------------------------------------- */
uint8_t enterPowerDown(_POWERDOWN_Type pwdMode);
#endif /* __VIOLET-BRANCH_PWR_H */

/************************ (C) COPYRIGHT Intsain.inc *****END OF FILE****/
