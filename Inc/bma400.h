
#ifndef BMA400_H
#define BMA400_H

#include "bma400.h"


/*#ifndef TRUE
#define TRUE     UINT8_C(1)
#endif

#ifndef FALSE
#define FALSE    UINT8_C(0)
#endif*/
/********************************************************/
/**\name Macro definitions */
/**\name API success code */
#define BMA400_OK       INT8_C(0)
/**\name API error codes */
#define BMA400_E_NULL_PTR          INT8_C(-1)
#define BMA400_E_COM_FAIL          INT8_C(-2)
#define BMA400_E_DEV_NOT_FOUND     INT8_C(-3)
#define BMA400_E_INVALID_CONFIG    INT8_C(-4)
/**\name API warning codes */
#define BMA400_W_SELF_TEST_FAIL    INT8_C(1)
/**\name CHIP ID VALUE */
#define BMA400_CHIP_ID     UINT8_C(0x90)
/********************************************************/
/**\name	USER CONFIGURATION MACROS               */
/**\name BMA400 I2C address macros */
#define BMA400_I2C_ADDRESS_SDO_LOW     UINT8_C(0x14)
#define BMA400_I2C_ADDRESS_SDO_HIGH    UINT8_C(0x15)
/**\name Power mode configurations */
#define BMA400_NORMAL_MODE       UINT8_C(0x02)
#define BMA400_SLEEP_MODE        UINT8_C(0x00)
#define BMA400_LOW_POWER_MODE    UINT8_C(0x01)
/**\name Enable / Disable macros */
#define BMA400_DISABLE        UINT8_C(0)
#define BMA400_ENABLE         UINT8_C(1)
/**\name Data/sensortime selection macros */
#define BMA400_DATA_ONLY          UINT8_C(0x00)
#define BMA400_DATA_SENSOR_TIME   UINT8_C(0x01)
/**\name ODR configurations  */
#define BMA400_ODR_12_5HZ     UINT8_C(0x05)
#define BMA400_ODR_25HZ       UINT8_C(0x06)
#define BMA400_ODR_50HZ       UINT8_C(0x07)
#define BMA400_ODR_100HZ      UINT8_C(0x08)
#define BMA400_ODR_200HZ      UINT8_C(0x09)
#define BMA400_ODR_400HZ      UINT8_C(0x0A)
#define BMA400_ODR_800HZ      UINT8_C(0x0B)
/**\name Accel Range configuration */
#define BMA400_2G_RANGE      UINT8_C(0x00)
#define BMA400_4G_RANGE      UINT8_C(0x01)
#define BMA400_8G_RANGE      UINT8_C(0x02)
#define BMA400_16G_RANGE     UINT8_C(0x03)
/**\name Accel Axes selection settings for
 * DATA SAMPLING, WAKEUP, ORIENTATION CHANGE,
 * GEN1, GEN2 , ACTIVITY CHANGE
 */
#define BMA400_X_AXIS_EN       UINT8_C(0x01)
#define BMA400_Y_AXIS_EN       UINT8_C(0x02)
#define BMA400_Z_AXIS_EN       UINT8_C(0x04)
#define BMA400_XYZ_AXIS_EN     UINT8_C(0x07)
/**\name Accel filter(data_src_reg) selection settings */
#define BMA400_DATA_SRC_ACCEL_FILT_1     UINT8_C(0x00)
#define BMA400_DATA_SRC_ACCEL_FILT_2     UINT8_C(0x01)
#define BMA400_DATA_SRC_ACCEL_FILT_LP    UINT8_C(0x02)
/**\name Accel OSR (OSR,OSR_LP) settings */
#define BMA400_ACCEL_OSR_SETTING_0       UINT8_C(0x00)
#define BMA400_ACCEL_OSR_SETTING_1       UINT8_C(0x01)
#define BMA400_ACCEL_OSR_SETTING_2       UINT8_C(0x02)
#define BMA400_ACCEL_OSR_SETTING_3       UINT8_C(0x03)
/**\name Accel filt1_bw settings */
/* Accel filt1_bw = 0.48 * ODR */
#define BMA400_ACCEL_FILT1_BW_0    UINT8_C(0x00)
/* Accel filt1_bw = 0.24 * ODR */
#define BMA400_ACCEL_FILT1_BW_1    UINT8_C(0x01)
/**\name Auto wake-up timeout value of 10.24s */
#define BMA400_AUTO_WAKEUP_TIMEOUT_MAX  UINT16_C(0x0FFF)
/**\name Auto low power timeout value of 10.24s */
#define BMA400_AUTO_LP_TIMEOUT_MAX      UINT16_C(0x0FFF)
/**\name Reference Update macros */
#define BMA400_MANUAL_UPDATE         UINT8_C(0x00)
#define BMA400_ONE_TIME_UPDATE       UINT8_C(0x01)
#define BMA400_EVERY_TIME_UPDATE     UINT8_C(0x02)
#define BMA400_LP_EVERY_TIME_UPDATE  UINT8_C(0x03)
/**\name Reference Update macros for orient interrupts */
#define BMA400_ORIENT_REFU_ACC_FILT_2       UINT8_C(0x01)
#define BMA400_ORIENT_REFU_ACC_FILT_LP      UINT8_C(0x02)
/**\name Stability evaluation macros for orient interrupts */
#define BMA400_STABILITY_DISABLED        UINT8_C(0x00)
#define BMA400_STABILITY_ACC_FILT_2      UINT8_C(0x01)
#define BMA400_STABILITY_ACC_FILT_LP     UINT8_C(0x02)
/**\name Number of samples needed for Auto-wakeup interrupt evaluation  */
#define BMA400_SAMPLE_COUNT_1   UINT8_C(0x00)
#define BMA400_SAMPLE_COUNT_2   UINT8_C(0x01)
#define BMA400_SAMPLE_COUNT_3   UINT8_C(0x02)
#define BMA400_SAMPLE_COUNT_4   UINT8_C(0x03)
#define BMA400_SAMPLE_COUNT_5   UINT8_C(0x04)
#define BMA400_SAMPLE_COUNT_6   UINT8_C(0x05)
#define BMA400_SAMPLE_COUNT_7   UINT8_C(0x06)
#define BMA400_SAMPLE_COUNT_8   UINT8_C(0x07)
/**\name Auto low power configurations */
/* Auto low power timeout disabled  */
#define BMA400_AUTO_LP_TIMEOUT_DISABLE    UINT8_C(0x00)
/* Auto low power entered on drdy interrupt */
#define BMA400_AUTO_LP_DRDY_TRIGGER       UINT8_C(0x01)
/* Auto low power entered on GEN1 interrupt */
#define BMA400_AUTO_LP_GEN1_TRIGGER       UINT8_C(0x02)
/* Auto low power entered on timeout of threshold value */
#define BMA400_AUTO_LP_TIMEOUT_EN         UINT8_C(0x04)
/* Auto low power entered on timeout of threshold value
 * but reset on activity detection
 */
#define BMA400_AUTO_LP_TIME_RESET_EN      UINT8_C(0x08)
/**\name    TAP INTERRUPT CONFIG MACROS   */
/* Axes select for TAP interrupt */
#define BMA400_X_AXIS_EN_TAP   UINT8_C(0x02)
#define BMA400_Y_AXIS_EN_TAP   UINT8_C(0x01)
#define BMA400_Z_AXIS_EN_TAP   UINT8_C(0x00)
/*! TAP tics_th setting */
/* Maximum time between upper and lower peak of a tap, in data samples
 * this time depends on the mechanics of the device tapped onto
 * default = 12 samples
 */
/* Configures 6 data samples for high-low tap signal change time */
#define BMA400_TICS_TH_6_DATA_SAMPLES  UINT8_C(0x00)
/* Configures 9 data samples for high-low tap signal change time */
#define BMA400_TICS_TH_9_DATA_SAMPLES  UINT8_C(0x01)
/* Configures 12 data samples for high-low tap signal change time */
#define BMA400_TICS_TH_12_DATA_SAMPLES UINT8_C(0x02)
/* Configures 18 data samples for high-low tap signal change time */
#define BMA400_TICS_TH_18_DATA_SAMPLES UINT8_C(0x03)
/*! TAP Sensitivity setting */
/* It modifies the threshold for minimum TAP amplitude */
/* BMA400_TAP_SENSITIVITY_0 correspond to highest sensitivity */
#define BMA400_TAP_SENSITIVITY_0  UINT8_C(0x00)
#define BMA400_TAP_SENSITIVITY_1  UINT8_C(0x01)
#define BMA400_TAP_SENSITIVITY_2  UINT8_C(0x02)
#define BMA400_TAP_SENSITIVITY_3  UINT8_C(0x03)
#define BMA400_TAP_SENSITIVITY_4  UINT8_C(0x04)
#define BMA400_TAP_SENSITIVITY_5  UINT8_C(0x05)
#define BMA400_TAP_SENSITIVITY_6  UINT8_C(0x06)
/* BMA400_TAP_SENSITIVITY_7 correspond to lowest sensitivity */
#define BMA400_TAP_SENSITIVITY_7  UINT8_C(0x07)
/*!  BMA400 TAP - quiet  settings */
/* Quiet refers to minimum quiet time before and after double tap,
 * in the data samples This time also defines the longest time interval
 * between two taps so that they are considered as double tap
 */
/* Configures 60 data samples quiet time between single or double taps */
#define BMA400_QUIET_60_DATA_SAMPLES   UINT8_C(0x00)
/* Configures 80 data samples quiet time between single or double taps */
#define BMA400_QUIET_80_DATA_SAMPLES   UINT8_C(0x01)
/* Configures 100 data samples quiet time between single or double taps */
#define BMA400_QUIET_100_DATA_SAMPLES  UINT8_C(0x02)
/* Configures 120 data samples quiet time between single or double taps */
#define BMA400_QUIET_120_DATA_SAMPLES  UINT8_C(0x03)
/*!  BMA400 TAP - quiet_dt  settings */
/* quiet_dt refers to Minimum time between the two taps of a
 * double tap, in data samples
 */
/* Configures 4 data samples minimum time between double taps */
#define BMA400_QUIET_DT_4_DATA_SAMPLES   UINT8_C(0x00)
/* Configures 8 data samples minimum time between double taps */
#define BMA400_QUIET_DT_8_DATA_SAMPLES   UINT8_C(0x01)
/* Configures 12 data samples minimum time between double taps */
#define BMA400_QUIET_DT_12_DATA_SAMPLES  UINT8_C(0x02)
/* Configures 16 data samples minimum time between double taps */
#define BMA400_QUIET_DT_16_DATA_SAMPLES  UINT8_C(0x03)
/**\name	ACTIVITY CHANGE CONFIG MACROS	*/
/* Data source for activity change detection */
#define BMA400_DATA_SRC_ACC_FILT1      UINT8_C(0x00)
#define BMA400_DATA_SRC_ACC_FILT2      UINT8_C(0x01)
/* Number of samples to evaluate for activity change detection */
#define BMA400_ACT_CH_SAMPLE_CNT_32    UINT8_C(0x00)
#define BMA400_ACT_CH_SAMPLE_CNT_64    UINT8_C(0x01)
#define BMA400_ACT_CH_SAMPLE_CNT_128   UINT8_C(0x02)
#define BMA400_ACT_CH_SAMPLE_CNT_256   UINT8_C(0x03)
#define BMA400_ACT_CH_SAMPLE_CNT_512   UINT8_C(0x04)
/**\name Interrupt pin configuration macros */
#define BMA400_INT_PUSH_PULL_ACTIVE_0    UINT8_C(0x00)
#define BMA400_INT_PUSH_PULL_ACTIVE_1    UINT8_C(0x01)
#define BMA400_INT_OPEN_DRIVE_ACTIVE_0   UINT8_C(0x02)
#define BMA400_INT_OPEN_DRIVE_ACTIVE_1   UINT8_C(0x03)
/**\name Interrupt Assertion status macros */
#define BMA400_WAKEUP_INT_ASSERTED      UINT16_C(0x0001)
#define BMA400_ORIENT_CH_INT_ASSERTED   UINT16_C(0x0002)
#define BMA400_GEN1_INT_ASSERTED        UINT16_C(0x0004)
#define BMA400_GEN2_INT_ASSERTED        UINT16_C(0x0008)
#define BMA400_INT_OVERRUN_ASSERTED     UINT16_C(0x0010)
#define BMA400_FIFO_FULL_INT_ASSERTED   UINT16_C(0x0020)
#define BMA400_FIFO_WM_INT_ASSERTED     UINT16_C(0x0040)
#define BMA400_DRDY_INT_ASSERTED        UINT16_C(0x0080)
#define BMA400_STEP_INT_ASSERTED        UINT16_C(0x0300)
#define BMA400_S_TAP_INT_ASSERTED       UINT16_C(0x0400)
#define BMA400_D_TAP_INT_ASSERTED       UINT16_C(0x0800)
#define BMA400_ACT_CH_X_ASSERTED        UINT16_C(0x2000)
#define BMA400_ACT_CH_Y_ASSERTED        UINT16_C(0x4000)
#define BMA400_ACT_CH_Z_ASSERTED        UINT16_C(0x8000)
/**\name Generic interrupt criterion_sel configuration macros */
#define BMA400_ACTIVITY_INT             UINT8_C(0x01)
#define BMA400_INACTIVITY_INT           UINT8_C(0x00)
/**\name Generic interrupt axes evaluation logic configuration macros */
#define BMA400_ALL_AXES_INT             UINT8_C(0x01)
#define BMA400_ANY_AXES_INT             UINT8_C(0x00)
/**\name Generic interrupt hysteresis configuration macros */
#define BMA400_HYST_0_MG     UINT8_C(0x00)
#define BMA400_HYST_24_MG    UINT8_C(0x01)
#define BMA400_HYST_48_MG    UINT8_C(0x02)
#define BMA400_HYST_96_MG    UINT8_C(0x03)
/**********************************************************************/
/**\name BMA400 Register Address */
#define BMA400_CHIP_ID_ADDR              UINT8_C(0x00)
#define BMA400_STATUS_ADDR               UINT8_C(0x03)
#define BMA400_ACCEL_DATA_ADDR           UINT8_C(0x04)
#define BMA400_INT_STAT0_ADDR            UINT8_C(0x0E)
#define BMA400_TEMP_DATA_ADDR            UINT8_C(0x11)
#define BMA400_FIFO_LENGTH_ADDR          UINT8_C(0x12)
#define BMA400_FIFO_DATA_ADDR            UINT8_C(0x14)
#define BMA400_STEP_CNT_0_ADDR           UINT8_C(0x15)
#define BMA400_ACCEL_CONFIG_0_ADDR       UINT8_C(0x19)
#define BMA400_ACCEL_CONFIG_1_ADDR       UINT8_C(0x1A)
#define BMA400_ACCEL_CONFIG_2_ADDR       UINT8_C(0x1B)
#define BMA400_INT_CONF_0_ADDR           UINT8_C(0x1F)
#define BMA400_INT_12_IO_CTRL_ADDR       UINT8_C(0x24)
#define BMA400_INT_MAP_ADDR              UINT8_C(0x21)
#define BMA400_FIFO_CONFIG_0_ADDR        UINT8_C(0x26)
#define BMA400_FIFO_READ_EN_ADDR         UINT8_C(0x29)
#define BMA400_AUTO_LOW_POW_0_ADDR       UINT8_C(0x2A)
#define BMA400_AUTO_LOW_POW_1_ADDR       UINT8_C(0x2B)
#define BMA400_AUTOWAKEUP_0_ADDR         UINT8_C(0x2C)
#define BMA400_AUTOWAKEUP_1_ADDR         UINT8_C(0x2D)
#define BMA400_WAKEUP_INT_CONF_0_ADDR    UINT8_C(0x2F)
#define BMA400_ORIENTCH_INT_CONFIG_ADDR  UINT8_C(0x35)
#define BMA400_GEN1_INT_CONFIG_ADDR      UINT8_C(0x3F)
#define BMA400_GEN2_INT_CONFIG_ADDR      UINT8_C(0x4A)
#define BMA400_ACT_CH_CONFIG_0_ADDR      UINT8_C(0x55)
#define BMA400_TAP_CONFIG_ADDR           UINT8_C(0x57)
#define BMA400_SELF_TEST_ADDR            UINT8_C(0x7D)
#define BMA400_COMMAND_REG_ADDR          UINT8_C(0x7E)
/**\name BMA400 Command register */
#define BMA400_SOFT_RESET_CMD     UINT8_C(0xB6)
#define BMA400_FIFO_FLUSH_CMD     UINT8_C(0xB0)
/**\name BMA400 Delay definitions */
#define BMA400_SOFT_RESET_DELAY_MS       UINT8_C(5)
#define BMA400_SELF_TEST_DELAY_MS        UINT8_C(7)
#define BMA400_SELF_TEST_DATA_READ_MS    UINT8_C(50)
/**\name Interface selection macro */
#define BMA400_SPI_WR_MASK    UINT8_C(0x7F)
#define BMA400_SPI_RD_MASK    UINT8_C(0x80)
/**\name UTILITY MACROS	*/
#define BMA400_SET_LOW_BYTE     UINT16_C(0x00FF)
#define BMA400_SET_HIGH_BYTE    UINT16_C(0xFF00)
/**\name Interrupt mapping selection */
#define BMA400_DATA_READY_INT_MAP     UINT8_C(0x01)
#define BMA400_FIFO_WM_INT_MAP        UINT8_C(0x02)
#define BMA400_FIFO_FULL_INT_MAP      UINT8_C(0x03)
#define BMA400_GEN2_INT_MAP           UINT8_C(0x04)
#define BMA400_GEN1_INT_MAP           UINT8_C(0x05)
#define BMA400_ORIENT_CH_INT_MAP      UINT8_C(0x06)
#define BMA400_WAKEUP_INT_MAP         UINT8_C(0x07)
#define BMA400_ACT_CH_INT_MAP         UINT8_C(0x08)
#define BMA400_TAP_INT_MAP            UINT8_C(0x09)
#define BMA400_STEP_INT_MAP           UINT8_C(0x0A)
#define BMA400_INT_OVERRUN_MAP        UINT8_C(0x0B)
/**\name BMA400 FIFO configurations */
#define BMA400_FIFO_AUTO_FLUSH          UINT8_C(0x01)
#define BMA400_FIFO_STOP_ON_FULL        UINT8_C(0x02)
#define BMA400_FIFO_TIME_EN             UINT8_C(0x04)
#define BMA400_FIFO_DATA_SRC            UINT8_C(0x08)
#define BMA400_FIFO_8_BIT_EN            UINT8_C(0x10)
#define BMA400_FIFO_X_EN                UINT8_C(0x20)
#define BMA400_FIFO_Y_EN                UINT8_C(0x40)
#define BMA400_FIFO_Z_EN                UINT8_C(0x80)
/**\name BMA400 FIFO data configurations */
#define BMA400_FIFO_EN_X         UINT8_C(0x01)
#define BMA400_FIFO_EN_Y         UINT8_C(0x02)
#define BMA400_FIFO_EN_Z         UINT8_C(0x04)
#define BMA400_FIFO_EN_XY        UINT8_C(0x03)
#define BMA400_FIFO_EN_YZ        UINT8_C(0x06)
#define BMA400_FIFO_EN_XZ        UINT8_C(0x05)
#define BMA400_FIFO_EN_XYZ       UINT8_C(0x07)
/**\name BMA400 Self test configurations */
#define BMA400_DISABLE_SELF_TEST          UINT8_C(0x00)
#define BMA400_ENABLE_POSITIVE_SELF_TEST  UINT8_C(0x07)
#define BMA400_ENABLE_NEGATIVE_SELF_TEST  UINT8_C(0x0F)
/**\name BMA400 FIFO data masks */
#define BMA400_FIFO_HEADER_MASK        UINT8_C(0x3E)
#define BMA400_FIFO_BYTES_OVERREAD     UINT8_C(25)
#define BMA400_AWIDTH_MASK             UINT8_C(0xEF)
#define BMA400_FIFO_DATA_EN_MASK       UINT8_C(0x0E)
/**\name BMA400 Step status field - Activity status */
#define BMA400_STILL_ACT     UINT8_C(0x00)
#define BMA400_WALK_ACT      UINT8_C(0x01)
#define BMA400_RUN_ACT       UINT8_C(0x02)
/*! It is inserted when FIFO_CONFIG0.fifo_data_src
 * is changed during the FIFO read
 */
#define BMA400_FIFO_CONF0_CHANGE  UINT8_C(0x01)
/*! It is inserted when ACC_CONFIG0.filt1_bw
 * is changed during the FIFO read
  */
#define BMA400_ACCEL_CONF0_CHANGE  UINT8_C(0x02)
/*! It is inserted when ACC_CONFIG1.acc_range
 * acc_odr or osr is changed during the FIFO read
 */
#define BMA400_ACCEL_CONF1_CHANGE  UINT8_C(0x04)
/*! Accel width setting either 12/8 bit mode */
#define BMA400_12_BIT_FIFO_DATA       UINT8_C(0x01)
#define BMA400_8_BIT_FIFO_DATA        UINT8_C(0x00)
/**\name BMA400 FIFO header configurations */
#define BMA400_FIFO_SENSOR_TIME     UINT8_C(0xA0)
#define BMA400_FIFO_EMPTY_FRAME     UINT8_C(0x80)
#define BMA400_FIFO_CONTROL_FRAME   UINT8_C(0x48)
#define BMA400_FIFO_XYZ_ENABLE      UINT8_C(0x8E)
#define BMA400_FIFO_X_ENABLE        UINT8_C(0x82)
#define BMA400_FIFO_Y_ENABLE        UINT8_C(0x84)
#define BMA400_FIFO_Z_ENABLE        UINT8_C(0x88)
#define BMA400_FIFO_XY_ENABLE       UINT8_C(0x86)
#define BMA400_FIFO_YZ_ENABLE       UINT8_C(0x8C)
#define BMA400_FIFO_XZ_ENABLE       UINT8_C(0x8A)
/**\name BMA400 bit mask definitions */
#define BMA400_POWER_MODE_STATUS_MSK         UINT8_C(0x06)
#define BMA400_POWER_MODE_STATUS_POS         UINT8_C(1)

#define BMA400_POWER_MODE_MSK      UINT8_C(0x03)

#define BMA400_ACCEL_ODR_MSK      UINT8_C(0x0F)

#define BMA400_ACCEL_RANGE_MSK    UINT8_C(0xC0)
#define BMA400_ACCEL_RANGE_POS    UINT8_C(6)

#define BMA400_DATA_FILTER_MSK   UINT8_C(0x0C)
#define BMA400_DATA_FILTER_POS   UINT8_C(2)

#define BMA400_OSR_MSK    UINT8_C(0x30)
#define BMA400_OSR_POS    UINT8_C(4)

#define BMA400_OSR_LP_MSK        UINT8_C(0x60)
#define BMA400_OSR_LP_POS        UINT8_C(5)

#define BMA400_FILT_1_BW_MSK            UINT8_C(0x80)
#define BMA400_FILT_1_BW_POS            UINT8_C(7)

#define BMA400_WAKEUP_TIMEOUT_MSK       UINT8_C(0x04)
#define BMA400_WAKEUP_TIMEOUT_POS       UINT8_C(2)

#define BMA400_WAKEUP_THRES_LSB_MSK     UINT16_C(0x000F)

#define BMA400_WAKEUP_THRES_MSB_MSK     UINT16_C(0x0FF0)
#define BMA400_WAKEUP_THRES_MSB_POS     UINT8_C(4)

#define BMA400_WAKEUP_TIMEOUT_THRES_MSK UINT8_C(0xF0)
#define BMA400_WAKEUP_TIMEOUT_THRES_POS UINT8_C(4)

#define BMA400_WAKEUP_INTERRUPT_MSK     UINT8_C(0x02)
#define BMA400_WAKEUP_INTERRUPT_POS     UINT8_C(1)

#define BMA400_AUTO_LOW_POW_MSK       UINT8_C(0x0F)

#define BMA400_AUTO_LP_THRES_MSK      UINT16_C(0x0FF0)
#define BMA400_AUTO_LP_THRES_POS      UINT8_C(4)

#define BMA400_AUTO_LP_THRES_LSB_MSK  UINT16_C(0x000F)

#define BMA400_WKUP_REF_UPDATE_MSK    UINT8_C(0x03)

#define BMA400_AUTO_LP_TIMEOUT_LSB_MSK UINT8_C(0xF0)
#define BMA400_AUTO_LP_TIMEOUT_LSB_POS UINT8_C(4)

#define BMA400_SAMPLE_COUNT_MSK        UINT8_C(0x1C)
#define BMA400_SAMPLE_COUNT_POS        UINT8_C(2)

#define BMA400_WAKEUP_EN_AXES_MSK      UINT8_C(0xE0)
#define BMA400_WAKEUP_EN_AXES_POS      UINT8_C(5)

#define BMA400_TAP_AXES_EN_MSK         UINT8_C(0x18)
#define BMA400_TAP_AXES_EN_POS         UINT8_C(3)

#define BMA400_TAP_QUIET_DT_MSK         UINT8_C(0x30)
#define BMA400_TAP_QUIET_DT_POS         UINT8_C(4)

#define BMA400_TAP_QUIET_MSK            UINT8_C(0x0C)
#define BMA400_TAP_QUIET_POS            UINT8_C(2)

#define BMA400_TAP_TICS_TH_MSK         UINT8_C(0x03)

#define BMA400_TAP_SENSITIVITY_MSK     UINT8_C(0X07)

#define BMA400_ACT_CH_AXES_EN_MSK      UINT8_C(0xE0)
#define BMA400_ACT_CH_AXES_EN_POS      UINT8_C(5)

#define BMA400_ACT_CH_DATA_SRC_MSK     UINT8_C(0x10)
#define BMA400_ACT_CH_DATA_SRC_POS     UINT8_C(4)

#define BMA400_ACT_CH_NPTS_MSK         UINT8_C(0x0F)

#define BMA400_INT_AXES_EN_MSK    UINT8_C(0xE0)
#define BMA400_INT_AXES_EN_POS    UINT8_C(5)

#define BMA400_INT_DATA_SRC_MSK   UINT8_C(0x10)
#define BMA400_INT_DATA_SRC_POS   UINT8_C(4)

#define BMA400_INT_REFU_MSK       UINT8_C(0x0C)
#define BMA400_INT_REFU_POS       UINT8_C(2)

#define BMA400_INT_HYST_MSK         UINT8_C(0x03)
#define BMA400_STABILITY_MODE_MSK   UINT8_C(0x03)

#define BMA400_GEN_INT_COMB_MSK      UINT8_C(0x01)

#define BMA400_GEN_INT_CRITERION_MSK  UINT8_C(0x02)
#define BMA400_GEN_INT_CRITERION_POS  UINT8_C(0x01)

#define BMA400_INT_PIN1_CONF_MSK   UINT8_C(0x06)
#define BMA400_INT_PIN1_CONF_POS   UINT8_C(1)

#define BMA400_INT_PIN2_CONF_MSK   UINT8_C(0x60)
#define BMA400_INT_PIN2_CONF_POS   UINT8_C(5)

#define BMA400_INT_STATUS_MSK     UINT8_C(0xE0)
#define BMA400_INT_STATUS_POS     UINT8_C(5)

#define BMA400_EN_DRDY_MSK  UINT8_C(0x80)
#define BMA400_EN_DRDY_POS  UINT8_C(7)

#define BMA400_EN_FIFO_WM_MSK  UINT8_C(0x40)
#define BMA400_EN_FIFO_WM_POS  UINT8_C(6)

#define BMA400_EN_FIFO_FULL_MSK  UINT8_C(0x20)
#define BMA400_EN_FIFO_FULL_POS  UINT8_C(5)

#define BMA400_EN_INT_OVERRUN_MSK  UINT8_C(0x10)
#define BMA400_EN_INT_OVERRUN_POS  UINT8_C(4)

#define BMA400_EN_GEN2_MSK  UINT8_C(0x08)
#define BMA400_EN_GEN2_POS  UINT8_C(3)

#define BMA400_EN_GEN1_MSK  UINT8_C(0x04)
#define BMA400_EN_GEN1_POS  UINT8_C(2)

#define BMA400_EN_ORIENT_CH_MSK  UINT8_C(0x02)
#define BMA400_EN_ORIENT_CH_POS  UINT8_C(1)

#define BMA400_EN_LATCH_MSK  UINT8_C(0x80)
#define BMA400_EN_LATCH_POS  UINT8_C(7)

#define BMA400_EN_ACTCH_MSK  UINT8_C(0x10)
#define BMA400_EN_ACTCH_POS  UINT8_C(4)

#define BMA400_EN_D_TAP_MSK  UINT8_C(0x08)
#define BMA400_EN_D_TAP_POS  UINT8_C(3)

#define BMA400_EN_S_TAP_MSK  UINT8_C(0x04)
#define BMA400_EN_S_TAP_POS  UINT8_C(2)

#define BMA400_EN_STEP_INT_MSK  UINT8_C(0x01)

#define BMA400_STEP_MAP_INT2_MSK  UINT8_C(0x10)
#define BMA400_STEP_MAP_INT2_POS  UINT8_C(4)

#define BMA400_EN_WAKEUP_INT_MSK  UINT8_C(0x01)

#define BMA400_TAP_MAP_INT1_MSK  UINT8_C(0x04)
#define BMA400_TAP_MAP_INT1_POS  UINT8_C(2)

#define BMA400_TAP_MAP_INT2_MSK  UINT8_C(0x40)
#define BMA400_TAP_MAP_INT2_POS  UINT8_C(6)

#define BMA400_ACTCH_MAP_INT1_MSK  UINT8_C(0x08)
#define BMA400_ACTCH_MAP_INT1_POS  UINT8_C(3)

#define BMA400_ACTCH_MAP_INT2_MSK  UINT8_C(0x80)
#define BMA400_ACTCH_MAP_INT2_POS  UINT8_C(7)

#define BMA400_FIFO_BYTES_CNT_MSK  UINT8_C(0x07)

#define BMA400_FIFO_TIME_EN_MSK  UINT8_C(0x04)
#define BMA400_FIFO_TIME_EN_POS  UINT8_C(2)

#define BMA400_FIFO_AXES_EN_MSK  UINT8_C(0xE0)
#define BMA400_FIFO_AXES_EN_POS  UINT8_C(5)

#define BMA400_FIFO_8_BIT_EN_MSK  UINT8_C(0x10)
#define BMA400_FIFO_8_BIT_EN_POS  UINT8_C(4)
/**\name Macro to SET and GET BITS of a register */
#define BMA400_SET_BITS(reg_data, bitname, data) \
	((reg_data & ~(bitname ## _MSK)) |	 \
	 ((data << bitname ## _POS) & bitname ## _MSK))

#define BMA400_GET_BITS(reg_data, bitname)  ((reg_data & (bitname ## _MSK)) >> \
					     (bitname ## _POS))

#define BMA400_SET_BITS_POS_0(reg_data, bitname, data) \
	((reg_data & ~(bitname ## _MSK)) |	       \
	 (data & bitname ## _MSK))

#define BMA400_GET_BITS_POS_0(reg_data, bitname)  (reg_data & (bitname ## _MSK))

#define BMA400_SET_BIT_VAL_0(reg_data, bitname) (reg_data & ~(bitname ## _MSK))

#define BMA400_GET_LSB(var)     (uint8_t)(var & BMA400_SET_LOW_BYTE)
#define BMA400_GET_MSB(var)     (uint8_t)((var & BMA400_SET_HIGH_BYTE) >> 8)
/********************************************************/

/** @}*/
/** @}*/

#define BMA400_ADDRESS                  0x15

#define BMA400_CHIPID                   0x00
#define BMA400_ERR_REG                  0x02
#define BMA400_STATUS                   0x03

#define BMA400_ACC_X_LSB                0x04
#define BMA400_ACC_X_MSB                0x05
#define BMA400_ACC_Y_LSB                0x06
#define BMA400_ACC_Y_MSB                0x07
#define BMA400_ACC_Z_LSB                0x08
#define BMA400_ACC_Z_MSB                0x09

#define BMA400_SENSOR_TIME_0            0x0A
#define BMA400_SENSOR_TIME_1            0x0B
#define BMA400_SENSOR_TIME_2            0x0C

#define BMA400_EVENT                    0x0D

#define BMA400_INT_STAT0                0x0E
#define BMA400_INT_STAT1                0x0F
#define BMA400_INT_STAT2                0x10

#define BMA400_TEMP_DATA                0x11

#define BMA400_FIFO_LENGTH_0            0x12
#define BMA400_FIFO_LENGTH_1            0x13
#define BMA400_FIFO_DATA                0x14

#define BMA400_STEP_CNT_0               0x15
#define BMA400_STEP_CNT_1               0x16
#define BMA400_STEP_CNT_2               0x17
#define BMA400_STEP_STAT                0x18

#define BMA400_ACC_CONFIG_0             0x19
#define BMA400_ACC_CONFIG_1             0x1A
#define BMA400_ACC_CONFIG_2             0x1B

#define BMA400_INT_CONFIG_0             0x1F
#define BMA400_INT_CONFIG_1             0x20
#define BMA400_INT_1_MAP                0x21
#define BMA400_INT_2_MAP                0x22
#define BMA400_INT_1_2_MAP              0x23
#define BMA400_INT_1_2_CTRL             0x24

#define BMA400_FIFO_CONFIG_0            0x26
#define BMA400_FIFO_CONFIG_1            0x27
#define BMA400_FIFO_CONFIG_2            0x28
#define BMA400_FIFO_PWR_CONFIG          0x29

#define BMA400_AUTO_LOW_POW_0           0x2A
#define BMA400_AUTO_LOW_POW_1           0x2B
#define BMA400_AUTO_WAKE_UP_0           0x2C
#define BMA400_AUTO_WAKE_UP_1           0x2D
#define BMA400_WAKE_UP_INT_CONFIG_0     0x2F
#define BMA400_WAKE_UP_INT_CONFIG_1     0x30
#define BMA400_WAKE_UP_INT_CONFIG_2     0x31
#define BMA400_WAKE_UP_INT_CONFIG_3     0x32
#define BMA400_WAKE_UP_INT_CONFIG_4     0x33

#define BMA400_ORIENTCH_CONFIG_0        0x35
#define BMA400_ORIENTCH_CONFIG_1        0x36
#define BMA400_ORIENTCH_CONFIG_2        0x37
#define BMA400_ORIENTCH_CONFIG_3        0x38
#define BMA400_ORIENTCH_CONFIG_4        0x39
#define BMA400_ORIENTCH_CONFIG_5        0x3A
#define BMA400_ORIENTCH_CONFIG_6        0x3B
#define BMA400_ORIENTCH_CONFIG_7        0x3C
#define BMA400_ORIENTCH_CONFIG_8        0x3D
#define BMA400_ORIENTCH_CONFIG_9        0x3E

#define BMA400_GEN_1_INT_CONFIG_0       0x3F
#define BMA400_GEN_1_INT_CONFIG_1       0x40
#define BMA400_GEN_1_INT_CONFIG_2       0x41
#define BMA400_GEN_1_INT_CONFIG_3       0x42
#define BMA400_GEN_1_INT_CONFIG_3_1     0x43
#define BMA400_GEN_1_INT_CONFIG_4       0x44
#define BMA400_GEN_1_INT_CONFIG_5       0x45
#define BMA400_GEN_1_INT_CONFIG_6       0x46
#define BMA400_GEN_1_INT_CONFIG_7       0x47
#define BMA400_GEN_1_INT_CONFIG_8       0x48
#define BMA400_GEN_1_INT_CONFIG_9       0x49

#define BMA400_GEN_2_INT_CONFIG_0       0x4A
#define BMA400_GEN_2_INT_CONFIG_1       0x4B
#define BMA400_GEN_2_INT_CONFIG_2       0x4C
#define BMA400_GEN_2_INT_CONFIG_3       0x4D
#define BMA400_GEN_2_INT_CONFIG_3_1     0x4E
#define BMA400_GEN_2_INT_CONFIG_4       0x4F
#define BMA400_GEN_2_INT_CONFIG_5       0x50
#define BMA400_GEN_2_INT_CONFIG_6       0x51
#define BMA400_GEN_2_INT_CONFIG_7       0x52
#define BMA400_GEN_2_INT_CONFIG_8       0x53
#define BMA400_GEN_2_INT_CONFIG_9       0x54

#define BMA400_ACT_CH_CONFIG_0          0x55
#define BMA400_ACT_CH_CONFIG_1          0x56

#define BMA400_TAP_CONFIG_0             0x57
#define BMA400_TAP_CONFIG_1             0x58

#define BMA400_STEP_COUNTER_CONFIG_0    0x59
#define BMA400_STEP_COUNTER_CONFIG_1    0x5A
#define BMA400_STEP_COUNTER_CONFIG_2    0x5B
#define BMA400_STEP_COUNTER_CONFIG_3    0x5C
#define BMA400_STEP_COUNTER_CONFIG_4    0x5D
#define BMA400_STEP_COUNTER_CONFIG_5    0x5E
#define BMA400_STEP_COUNTER_CONFIG_6    0x5F
#define BMA400_STEP_COUNTER_CONFIG_7    0x60
#define BMA400_STEP_COUNTER_CONFIG_8    0x61
#define BMA400_STEP_COUNTER_CONFIG_9    0x62
#define BMA400_STEP_COUNTER_CONFIG_10   0x63
#define BMA400_STEP_COUNTER_CONFIG_11   0x64
#define BMA400_STEP_COUNTER_CONFIG_12   0x65
#define BMA400_STEP_COUNTER_CONFIG_13   0x66
#define BMA400_STEP_COUNTER_CONFIG_14   0x67
#define BMA400_STEP_COUNTER_CONFIG_15   0x68
#define BMA400_STEP_COUNTER_CONFIG_16   0x69
#define BMA400_STEP_COUNTER_CONFIG_17   0x6A
#define BMA400_STEP_COUNTER_CONFIG_18   0x6B
#define BMA400_STEP_COUNTER_CONFIG_19   0x6C
#define BMA400_STEP_COUNTER_CONFIG_20   0x6D
#define BMA400_STEP_COUNTER_CONFIG_21   0x6E
#define BMA400_STEP_COUNTER_CONFIG_22   0x6F
#define BMA400_STEP_COUNTER_CONFIG_23   0x70
#define BMA400_STEP_COUNTER_CONFIG_24   0x71

#define BMA400_IF_CONF                  0x7C
#define BMA400_SELF_TEST                0x7D
#define BMA400_CMD                      0x7E

enum power_type_t // power mode
{
	SLEEP = 0x00, // Stop conversion, the lowest power mode
	LOW_POWER = 0x01, // 
	NORMAL = 0x02, // 
};

enum scale_type_t // measurement rage
{
	RANGE_2G = 0x00, // 
	RANGE_4G = 0x01, // 
	RANGE_8G = 0x02, // 
  RANGE_16G = 0x03, // 
};

enum odr_type_t // output data rate
{
	ODR_12 = 0x00, // 
	ODR_25 = 0x06, // 
	ODR_50 = 0x07, // 
	ODR_100 = 0x08, // 
	ODR_200 = 0x09, // 
	ODR_400 = 0x0A, // 
	ODR_800 = 0x0B, // 
};

void bma400_spi_init(void);
void getAcceleration(float* x, float* y, float* z);
void BMA400_initialize(void);

#endif //BMA400_H
