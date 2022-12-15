
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "bme280.h"
#include "stm32l4xx_hal.h"
#include "vcom.h"

extern I2C_HandleTypeDef hi2c1;

BME280Settings settings;
BME280Calibration calibration;
int32_t t_fine;

static uint8_t BME289_reg_get(uint8_t reg_addr)
{
  
  
  uint8_t value=0;  
  uint8_t twi_addr = BME280_I2C_ADDRESS;//m_drv_ccs811.p_drv_ccs811_cfg->twi_addr;

  //nrf_drv_twi_tx(p_instance, twi_addr, &reg_addr, 1, M_TWI_SUSPEND);
  //nrf_drv_twi_rx(p_instance, twi_addr, &value, 1);
  
  HAL_I2C_Mem_Read(&hi2c1, twi_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, &value, 1, 10000); 

  return value;
}
/**@brief Write value to register.
 */
static uint8_t BME280_reg_set(uint8_t reg_addr, uint8_t value)
{
      
  uint8_t twi_addr   = BME280_I2C_ADDRESS;//m_drv_ccs811.p_drv_ccs811_cfg->twi_addr;
  uint8_t tx_buffer = value;

      //return ( nrf_drv_twi_tx(p_instance, twi_addr, &(tx_buffer[0]), 2, M_TWI_STOP) == NRF_SUCCESS );
  
  return (HAL_I2C_Mem_Write(&hi2c1, twi_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, &tx_buffer, 1, 10000));
}

uint8_t init_bme280(void){

  HAL_Delay(10);
          //Select interface mode
  settings.commInterface = 0; //Can be I2C_MODE, SPI_MODE
  //Select address for I2C.  Does nothing for SPI
  settings.I2CAddress = 0x77; //Ignored for SPI_MODE
  //Select CS pin for SPI.  Does nothing for I2C
  settings.chipSelectPin = 10;
  settings.runMode = 0;
  settings.tempOverSample = 0;
  settings.pressOverSample = 0;
  settings.humidOverSample = 0;


	
#if 1
//***Driver settings********************************//
//specify chipSelectPin using arduino pin names
//specify I2C address.  Can be 0x77(default) or 0x76
  
  //***Operation settings*****************************//
  
  //renMode can be:
  //  0, Sleep mode
  //  1 or 2, Forced mode
  //  3, Normal mode
  settings.runMode = 3; //Normal mode
  
  //tStandby can be:
  //  0, 0.5ms
  //  1, 62.5ms
  //  2, 125ms
  //  3, 250ms
  //  4, 500ms
  //  5, 1000ms
  //  6, 10ms
  //  7, 20ms
  settings.tStandby = 0;
  
  //filter can be off or number of FIR coefficients to use:
  //  0, filter off
  //  1, coefficients = 2
  //  2, coefficients = 4
  //  3, coefficients = 8
  //  4, coefficients = 16
  settings.filter = 0;
  
  //tempOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  settings.tempOverSample = 1;

  //pressOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  settings.pressOverSample = 1;
	
  //humidOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  settings.humidOverSample = 1;
#endif

  //Reading all compensation data, range 0x88:A1, 0xE1:E7  
  calibration.dig_T1 = ((uint16_t)((BME289_reg_get(BME280_DIG_T1_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_T1_LSB_REG)));
  calibration.dig_T2 = ((int16_t)((BME289_reg_get(BME280_DIG_T2_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_T2_LSB_REG)));
  calibration.dig_T3 = ((int16_t)((BME289_reg_get(BME280_DIG_T3_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_T3_LSB_REG)));

  calibration.dig_P1 = ((uint16_t)((BME289_reg_get(BME280_DIG_P1_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P1_LSB_REG)));
  calibration.dig_P2 = ((int16_t)((BME289_reg_get(BME280_DIG_P2_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P2_LSB_REG)));
  calibration.dig_P3 = ((int16_t)((BME289_reg_get(BME280_DIG_P3_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P3_LSB_REG)));
  calibration.dig_P4 = ((int16_t)((BME289_reg_get(BME280_DIG_P4_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P4_LSB_REG)));
  calibration.dig_P5 = ((int16_t)((BME289_reg_get(BME280_DIG_P5_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P5_LSB_REG)));
  calibration.dig_P6 = ((int16_t)((BME289_reg_get(BME280_DIG_P6_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P6_LSB_REG)));
  calibration.dig_P7 = ((int16_t)((BME289_reg_get(BME280_DIG_P7_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P7_LSB_REG)));
  calibration.dig_P8 = ((int16_t)((BME289_reg_get(BME280_DIG_P8_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P8_LSB_REG)));
  calibration.dig_P9 = ((int16_t)((BME289_reg_get(BME280_DIG_P9_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_P9_LSB_REG)));

  calibration.dig_H1 = ((uint8_t)(BME289_reg_get(BME280_DIG_H1_REG)));
  calibration.dig_H2 = ((int16_t)((BME289_reg_get(BME280_DIG_H2_MSB_REG) << 8) + BME289_reg_get(BME280_DIG_H2_LSB_REG)));
  calibration.dig_H3 = ((uint8_t)(BME289_reg_get(BME280_DIG_H3_REG)));
  calibration.dig_H4 = ((int16_t)((BME289_reg_get(BME280_DIG_H4_MSB_REG) << 4) + (BME289_reg_get(BME280_DIG_H4_LSB_REG) & 0x0F)));
  calibration.dig_H5 = ((int16_t)((BME289_reg_get(BME280_DIG_H5_MSB_REG) << 4) + ((BME289_reg_get(BME280_DIG_H4_LSB_REG) >> 4) & 0x0F)));
  calibration.dig_H6 = ((uint8_t)BME289_reg_get(BME280_DIG_H6_REG));

  uint8_t dataToWrite = 0;  //Temporary variable
//Set the oversampling control words.
  //config will only be writeable in sleep mode, so first insure that.
  BME280_reg_set(BME280_CTRL_MEAS_REG, 0x00);
  
  //Set the config word
  dataToWrite = (settings.tStandby << 0x5) & 0xE0;
  dataToWrite |= (settings.filter << 0x02) & 0x1C;
  BME280_reg_set(BME280_CONFIG_REG, dataToWrite);
  
  //Set ctrl_hum first, then ctrl_meas to activate ctrl_hum
  dataToWrite = settings.humidOverSample & 0x07; //all other bits can be ignored
  BME280_reg_set(BME280_CTRL_HUMIDITY_REG, dataToWrite);
  
  //set ctrl_meas
  //First, set temp oversampling
  dataToWrite = (settings.tempOverSample << 0x5) & 0xE0;
  //Next, pressure oversampling
  dataToWrite |= (settings.pressOverSample << 0x02) & 0x1C;
  //Last, set mode
  dataToWrite |= (settings.runMode) & 0x03;
  //Load the byte
  BME280_reg_set(BME280_CTRL_MEAS_REG, dataToWrite);
  
  return BME289_reg_get(0xD0);
}

//Strictly resets.  Run .begin() afterwards
void BME280reset( void )
{
  BME280_reg_set(BME280_RST_REG, 0xB6);	
}

void BME280SensorInfo(void){	
  PRINTF("ID(0xD0): 0x%x\r\n",BME289_reg_get(BME280_CHIP_ID_REG));	
  PRINTF("Reset register(0xE0) : 0x%x \r\n",BME289_reg_get(BME280_RST_REG));
  PRINTF("ctrl_meas(0xF4): 0x%x \r\n",BME289_reg_get(BME280_CTRL_MEAS_REG));	
  PRINTF("ctrl_hum(0xF2): 0x%x \r\n",BME289_reg_get(BME280_CTRL_HUMIDITY_REG));
}
//****************************************************************************//
//
//  Temperature Section
//
//****************************************************************************//

float BME280readTempC( void )
{
  // Returns temperature in DegC, resolution is 0.01 DegC. Output value of ¡°5123¡± equals 51.23 DegC.
  // t_fine carries fine temperature as global value

  //get the reading (adc_T);
  int32_t adc_T = ((uint32_t)BME289_reg_get(BME280_TEMPERATURE_MSB_REG) << 12) | ((uint32_t)BME289_reg_get(BME280_TEMPERATURE_LSB_REG) << 4) | ((BME289_reg_get(BME280_TEMPERATURE_XLSB_REG) >> 4) & 0x0F);

  //By datasheet, calibrate
  int64_t var1, var2;

  var1 = ((((adc_T>>3) - ((int32_t)calibration.dig_T1<<1))) * ((int32_t)calibration.dig_T2)) >> 11;
  var2 = (((((adc_T>>4) - ((int32_t)calibration.dig_T1)) * ((adc_T>>4) - ((int32_t)calibration.dig_T1))) >> 12) *
  ((int32_t)calibration.dig_T3)) >> 14;
  t_fine = var1 + var2;
  float output = (t_fine * 5 + 128) >> 8;

  output = output / 100;
  
  return output;
}
//****************************************************************************//
//
//  Pressure Section
//
//****************************************************************************//
float BME280readFloatPressure( void )
{
  // Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
  // Output value of ¡°24674867¡± represents 24674867/256 = 96386.2 Pa = 963.862 hPa
  int32_t adc_P = ((uint32_t)BME289_reg_get(BME280_PRESSURE_MSB_REG) << 12) | ((uint32_t)BME289_reg_get(BME280_PRESSURE_LSB_REG) << 4) | ((BME289_reg_get(BME280_PRESSURE_XLSB_REG) >> 4) & 0x0F);
  
  int64_t var1, var2, p_acc;
  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)calibration.dig_P6;
  var2 = var2 + ((var1 * (int64_t)calibration.dig_P5)<<17);
  var2 = var2 + (((int64_t)calibration.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)calibration.dig_P3)>>8) + ((var1 * (int64_t)calibration.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)calibration.dig_P1)>>33;
  if (var1 == 0)
  {
    return 0; // avoid exception caused by division by zero
  }
  p_acc = 1048576 - adc_P;
  p_acc = (((p_acc<<31) - var2)*3125)/var1;
  var1 = (((int64_t)calibration.dig_P9) * (p_acc>>13) * (p_acc>>13)) >> 25;
  var2 = (((int64_t)calibration.dig_P8) * p_acc) >> 19;
  p_acc = ((p_acc + var1 + var2) >> 8) + (((int64_t)calibration.dig_P7)<<4);
  
  return (float)p_acc / 256.0;
	
}
float BME280readFloatAltitudeMeters( void )
{
  float heightOutput = 0;
  
  heightOutput = ((float)-45846.2)*(pow(((float)BME280readFloatPressure()/(float)101325), 0.190263) - (float)1);
  return heightOutput;
	
}
//****************************************************************************//
//
//  Humidity Section
//
//****************************************************************************//
float BME280readFloatHumidity( void )
{
	
  // Returns humidity in %RH as unsigned 32 bit integer in Q22. 10 format (22 integer and 10 fractional bits).
  // Output value of ¡°47445¡± represents 47445/1024 = 46. 333 %RH
  int32_t adc_H = ((uint32_t)BME289_reg_get(BME280_HUMIDITY_MSB_REG) << 8) | ((uint32_t)BME289_reg_get(BME280_HUMIDITY_LSB_REG));
  
  int32_t var1;
  var1 = (t_fine - ((int32_t)76800));
  var1 = (((((adc_H << 14) - (((int32_t)calibration.dig_H4) << 20) - (((int32_t)calibration.dig_H5) * var1)) +
  ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)calibration.dig_H6)) >> 10) * (((var1 * ((int32_t)calibration.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
  ((int32_t)calibration.dig_H2) + 8192) >> 14));
  var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)calibration.dig_H1)) >> 4));
  var1 = (var1 < 0 ? 0 : var1);
  var1 = (var1 > 419430400 ? 419430400 : var1);

  return (float)(var1>>12) / 1024.0;

}
void BME280_init(void){
  init_bme280();

  BME280SensorInfo();

  //printf("BME280 init ok\r\n" );
}



