/**
 * @author Aaron Berk
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * ADXL345, triple axis, digital interface, accelerometer.
 *
 * Datasheet:
 *
 * http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf
 */

/**
 * Includes
 */
#include "ADXL345.h"

uint8_t spi_write (uint8_t byte) {
      //Wait for SPIy Tx buffer empty
while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    SPI_I2S_SendData(SPI1, byte);


while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET)
    return SPI_I2S_ReceiveData(SPI1);

/*
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
		;
    SPI_I2S_SendData(SPI1, byte);

    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
    	;
    return SPI_I2S_ReceiveData(SPI1);
    */
}
uint8_t ADXL345oneByteRead(uint8_t address) {


   // uint8_t tx = (ADXL345_SPI_READ | (address & 0x3F));
   uint8_t tx = address | ADXL345_SPI_READ;
    uint8_t rx = 0;

  GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    //Send address to read from.
    spi_write(tx);

    SPI1->CR1 |= (1<<14); // одиниц€ в 14 позицыъ
     SPI1->CR1 |= (1<<6);
    //Read back contents of address.
    rx = spi_write(0x00);
    SPI1->CR1 &= ~(1<<14); // нуль в 14 позицыъ
    SPI1->CR1 |= (1<<6);
     GPIO_SetBits(GPIOA, GPIO_Pin_1);


    return rx;

}

void ADXL345oneByteWrite(uint8_t address, char data) {

    //uint8_t tx = (ADXL345_SPI_WRITE | (address & 0x3F));
uint8_t tx = address | ADXL345_SPI_WRITE;
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    //Send address to write to.
    spi_write(tx);
    //Send data to be written.
    spi_write(data);
     GPIO_SetBits(GPIOA, GPIO_Pin_1);

}

void ADXL345multiByteRead(uint8_t startAddress, char* buffer, uint8_t size) {

    uint8_t tx = (ADXL345_SPI_READ | ADXL345_MULTI_BYTE | (startAddress & 0x3F));

    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    //Send address to start reading from.
    spi_write(tx);
uint8_t i;
    for (i = 0; i < size; i++) {
        buffer[i] = spi_write(0x00);
    }

    GPIO_SetBits(GPIOA, GPIO_Pin_1);

}

void ADXL345multiByteWrite(uint8_t startAddress, char* buffer, uint8_t size) {

    uint8_t tx = (ADXL345_SPI_WRITE | ADXL345_MULTI_BYTE | (startAddress & 0x3F));

     GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    //Send address to start reading from.
    spi_write(tx);
uint8_t i ;
    for (i = 0; i < size; i++) {
        buffer[i] = spi_write(0x00);
    }

     GPIO_SetBits(GPIOA, GPIO_Pin_1);

}
