/** \file max31913.c ******************************************************
*
* Project: MAXREFDES64#
* Filename: max31913.c
* Description:	This module is an embedded controller driver for the MAXREFDES64#.
* 							It contains high level functions:
*                      maxim_spi_16bit_transfer
*
*								This driver can be dropped into a user's application as a starting
*								point for development of an end application
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
* to deal in the Software without reps_striction, including without limitation
* the rights to use, copy, modify, merge, publish, dips_stribute, sublicense,
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

#include "max31913.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

uint16_t maxim_spi_16bit_transfer(uint16_t u_mosi)
	/**
* \brief				SPI 16-bit transfer 
*
* \param[in]		u_mosi: SPI transmit data 
*
* \retval				SPI receive data 
*/
{
	uint16_t u_miso;
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);	//NCS = 0
	SPI_I2S_SendData(SPI2, u_mosi);
	/* Wait for SPI2 data reception */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	u_miso=SPI_I2S_ReceiveData(SPI2);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);	//NCS = 1
	return u_miso;
}
