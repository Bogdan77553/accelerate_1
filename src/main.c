/*
**
**                           Main.c
**
**http://www.compel.ru/wordpress/wp-content/uploads/2010/07/Laboratornyiy_praktikum-dlya-STM32F3-i-STM32F4.pdf
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f10x_conf.h"
#include "ADXL345.cpp"
//    #define DUMMY 0x00

unsigned short temp;
char out[16];
int readings[3] = {0, 0, 0};                       // X, Y and Z

void init(void);
void init_leds(void);
void delay(unsigned long p)
{
	while(p>0){p--;}
}


int main(void) {

init();
init_leds();
GPIO_SetBits(GPIOA, GPIO_Pin_1);
//delay(10000000);
 ADXL345oneByteWrite(ADXL345_POWER_CTL_REG, 0x08);
  // Go into standby mode to configure the device.
  ADXL345oneByteWrite(ADXL345_POWER_CTL_REG, 0x00);
   // Measurement mode.
  ADXL345oneByteWrite(ADXL345_POWER_CTL_REG, 0x08);

  // Full resolution, +/-16g, 4mg/LSB.
  ADXL345oneByteWrite(ADXL345_DATA_FORMAT_REG, 0x40);

  // Set data rate.
 // ADXL345oneByteWrite(ADXL345_BW_RATE_REG, ADXL345_3200HZ);

readings[0] = ADXL345oneByteRead(ADXL345_DEVID_REG);

  while (1) {

    // Read X Hi
    readings[0] = ADXL345oneByteRead(ADXL345_DATAX0_REG) << 8;

    // Read X Lo
    readings[0] = readings[0] | ADXL345oneByteRead(ADXL345_DATAX1_REG);

    // Read Y Hi
    readings[1] = ADXL345oneByteRead(ADXL345_DATAY0_REG) << 8;

    // Read Y Lo
    readings[1] = readings[1] | ADXL345oneByteRead(ADXL345_DATAY1_REG);

    // Read Z Hi
    readings[2] = ADXL345oneByteRead(ADXL345_DATAZ0_REG) << 8;

    // Read Z Lo
    readings[2] = readings[2] | ADXL345oneByteRead(ADXL345_DATAZ1_REG);
 }
}

void init(void) {
GPIO_InitTypeDef gpio_init;
SPI_InitTypeDef spi_init;


RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
GPIO_StructInit(&gpio_init);
                                //   SCK           MOSI         //NSS
gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_7; //GPIO_Pin_4 |
gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &gpio_init);
         //СS
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &gpio_init);


SPI_StructInit(&spi_init);
spi_init.SPI_Mode = SPI_Mode_Master;
spi_init.SPI_CPHA = SPI_CPHA_2Edge;//Фаза тактового сигнала
spi_init.SPI_CPOL = SPI_CPOL_High; //Полярность тактового сигнала (В неактивном состоянии SCK = 1,тактовый сигнал имеет отметочный уровень “0)
//соответствует доке на ADXL "clock polarity (CPOL) = 1 and clock phase (CPHA) = 1".
spi_init.SPI_DataSize = SPI_DataSize_8b;
spi_init.SPI_FirstBit = SPI_FirstBit_LSB;
spi_init.SPI_NSS = SPI_NSS_Soft;
spi_init.SPI_Direction = SPI_Direction_1Line_Rx;
//Baud rate = Fpclk/256
// 36MHz/256=140kHz < 400kHz
spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
SPI_Init(SPI1, &spi_init);
SPI_Cmd(SPI1, ENABLE);
SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}
void init_leds(void) {
GPIO_InitTypeDef gpio_init;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
gpio_init.GPIO_Pin = GPIO_Pin_1;
GPIO_Init(GPIOB, &gpio_init);
}

