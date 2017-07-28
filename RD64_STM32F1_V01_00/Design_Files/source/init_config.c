/** \file init_config.c ******************************************************
*
* Project: MAXREFDES64#
* Filename: init_config.c
* Description:	This module contains all the functions used to
* 							initialize the STM32F1 peripherals
*
* Revision History:
*\n 10/07/2014 Rev 01.00 MG Initial release.
*\n
*
* --------------------------------------------------------------------
*
* This code follows the following naming conventions:
*
*\n char							ch_pmod_value
*\n char (array)			s_pmod_string[16]
*\n float							f_pmod_value
*\n int								n_pmod_value
*\n int (array)				an_pmod_value[16]
*\n u16								u_pmod_value
*\n u16 (array)				au_pmod_value[16]
*\n u8								uch_pmod_value
*\n u8 (array)				auch_pmod_buffer[16]
*\n unsigned int			un_pmod_value
*\n int *							pun_pmod_value
*
* ------------------------------------------------------------------------- */
/*
* Copyright (C) 2012 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED PRODUCTS BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated Products
* shall not be used except as stated in the Maxim Integrated Products
* Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products retains all ownership rights.
*
***************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "init_config.h"

GPIO_InitTypeDef gpio_init_structure;	//GPIO structure
SPI_InitTypeDef spi_init_structure; //SPI structure
USART_InitTypeDef usart_init_structure; //USART structure

void maxim_uart_init(void)
/**
* \brief Initialize the UART peripheral
* \par Details
* This function initializes the UART peripheral that connects to the terminal program.
* \n The baud rate is set to 460800
*
* \param None
*
* \retval None
*/
{
	usart_init_structure.USART_BaudRate = 921600;
  usart_init_structure.USART_WordLength = USART_WordLength_8b;
  usart_init_structure.USART_StopBits = USART_StopBits_1;
  usart_init_structure.USART_Parity = USART_Parity_No;
  usart_init_structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usart_init_structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  /* Configure USART Tx as alternate function push-pull */
  gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
  gpio_init_structure.GPIO_Pin = GPIO_Pin_9;
  gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpio_init_structure);

  /* Configure USART Rx as input floating */
  gpio_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpio_init_structure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &gpio_init_structure);

  /* USART configuration */
  USART_Init(USART1, &usart_init_structure);
    
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}


void maxim_gpio_init(void)
/**
* \brief Initialize the GPIO peripheral
* \par Details
* This function initializes the GPIO that are used in this application
*
* \param None
*
* \retval None
*/
{
	/* GPIOA Periph clock enable */
	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Configure PA0 (LED_G), PA15 (LED_R) in output pushpull mode */

	gpio_init_structure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_15;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpio_init_structure);
	
	
	/* Configure PB12 (NCS) in output pushpull mode */
	gpio_init_structure.GPIO_Pin = GPIO_Pin_12;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpio_init_structure);

	gpio_init_structure.GPIO_Pin = GPIO_Pin_5;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &gpio_init_structure);

	/* Output clock on MCO pin ---------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_8;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init_structure);
	RCC_MCOConfig(RCC_MCO_SYSCLK);  // Put on MCO pin the: System clock selected  
}



void maxim_spi_init(void)
/**
* \brief Initialize the SPI peripheral
* \par Details
* This function initializes the SPI2 peripheral
* \n CS is controlled by GPIO PB12
*
* \param None
*
* \retval None
*/
{
  // Enable the SPI periph 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  // Enable SCK, MOSI and MISO GPIO clocks 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
	/* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
	gpio_init_structure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
  gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;

	/* Configure SCK and MOSI pins as Alternate Function Push Pull */
	gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
	
  GPIO_Init(GPIOB, &gpio_init_structure);

  gpio_init_structure.GPIO_Pin = GPIO_Pin_14;

	/* Configure MISO pin as Input Floating  */
	gpio_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &gpio_init_structure);
	
	/* SPI2 Config -------------------------------------------------------------*/
  spi_init_structure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  spi_init_structure.SPI_Mode = SPI_Mode_Master;
  spi_init_structure.SPI_DataSize = SPI_DataSize_16b;
  spi_init_structure.SPI_CPOL = SPI_CPOL_Low;
  spi_init_structure.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init_structure.SPI_NSS = SPI_NSS_Soft;
  spi_init_structure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  spi_init_structure.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_init_structure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &spi_init_structure);
	
	/* Enable SPI2 */
  SPI_Cmd(SPI2, ENABLE);
}
