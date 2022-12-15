
#include "stm32l4xx_hal.h"
#include "bma400.h"
#include "vcom.h"

extern SPI_HandleTypeDef hspi1;

static float accRange;

uint8_t spi_read8(uint8_t reg){
	
  uint8_t buffer[16]={0,};
  uint8_t read_buf[16]={0,};
  
  buffer[0] = reg|BMA400_SPI_RD_MASK;
  
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)buffer, (uint8_t *)read_buf, 3, 5000) == HAL_OK )
     /* PRINTF("SPI ok\r\n");
  else
    PRINTF("SPI fail\r\n");*/
  //nrf_drv_spi_transfer(&spi, buffer, 3, read_buf, 3);		
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
  /*	PRINTF("read buf : ");
  for(int i=0; i< 16; i++){
          PRINTF("0x%x ",read_buf[i]);
  }
  PRINTF("\r\n");*/
  return read_buf[2];

}
void spi_write8(uint8_t reg, uint8_t data){
  uint8_t buffer[3]={0,};
  uint8_t read_buf[3]={0,};
	
  buffer[0]=reg;//|BMA400_SPI_WR_MASK;
  buffer[1]=data;
  
  // take the chip select low to select the device:
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)buffer, (uint8_t *)read_buf, 3, 5000) == HAL_OK )
      /*PRINTF("SPI ok\r\n");
  else
    PRINTF("SPI fail\r\n");*/
  //nrf_drv_spi_transfer(&spi, buffer, 3, read_buf, 3);		
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  
}
uint8_t getDeviceID(void)
{
    return spi_read8(BMA400_CHIPID);
}
void setPoweMode(uint8_t mode)
{
    uint8_t data = 0;
    
    data = spi_read8(BMA400_ACC_CONFIG_0);
    data = (data & 0xfc) | mode;
    spi_write8(BMA400_ACC_CONFIG_0, data);
}

void setFullScaleRange(enum scale_type_t range)
{
  uint8_t data = 0;
    
  if(range == RANGE_2G) accRange = 2000;
  else if(range == RANGE_4G) accRange = 4000;
  else if(range == RANGE_8G) accRange = 8000;
  else if(range == RANGE_16G) accRange = 16000;
  
  data = spi_read8(BMA400_ACC_CONFIG_1);
  data = (data & 0x3f) | (range << 6);
  spi_write8(BMA400_ACC_CONFIG_1, data);
}

void setOutputDataRate(enum odr_type_t odr)
{
  uint8_t data = 0;
  
  data = spi_read8(BMA400_ACC_CONFIG_1);
  data = (data & 0xf0) | odr;
  spi_write8(BMA400_ACC_CONFIG_1, data);
}

void BMA400_initialize(void)
{
  PRINTF("bma400 dev id : 0x%x \r\n", getDeviceID());
  setPoweMode(LOW_POWER);    
  setFullScaleRange(RANGE_16G);
  setOutputDataRate(ODR_200);
}
int8_t spi_read(uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
    
  uint8_t buffer[16]={0,};
  uint8_t read_buf[16]={0,};
  
  buffer[0] = reg_addr|BMA400_SPI_RD_MASK;
  
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)buffer, (uint8_t *)read_buf, 16, 5000) == HAL_OK )
   /* PRINTF("SPI ok\r\n");
  else
    PRINTF("SPI fail\r\n");*/
  //nrf_drv_spi_transfer(&spi, buffer, 3, read_buf, 3);		
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  
  
  /*nrf_gpio_pin_clear(SPI_CS_PIN);
  nrf_drv_spi_transfer(&spi, buffer, 16, read_buf, 16);		
  
  nrf_gpio_pin_set(SPI_CS_PIN);
  */
  memcpy(reg_data, &read_buf[2], length);
  /*PRINTF("read buf : ");
  for(int i=0; i< 16; i++){
                          PRINTF("0x%x ",read_buf[i]);
  }
  PRINTF("\r\n");*/
  //PRINTF("0x%x 0x%x len= %d\r\n", reg_data[2], read_buf[2],length);
  /* Read from registers using SPI. Return 0 for a successful execution. */
  return 0;
}
void getAcceleration(float* x, float* y, float* z)
{
  uint8_t buf[6] = {0};
  uint16_t ax = 0, ay = 0, az = 0;
  float value = 0;
  
  spi_read(BMA400_ACC_X_LSB, buf, 6);
  
  ax = buf[0] | (buf[1] << 8);
  ay = buf[2] | (buf[3] << 8);
  az = buf[4] | (buf[5] << 8);
  
  if(ax > 2047) ax = ax - 4096;
  if(ay > 2047) ay = ay - 4096;
  if(az > 2047) az = az - 4096;
 
  value = (int16_t)ax;
  *x = accRange * value / 2048;
  
  value = (int16_t)ay;
  *y = accRange * value / 2048;
  
  value = (int16_t)az;
  *z = accRange * value / 2048;
}

