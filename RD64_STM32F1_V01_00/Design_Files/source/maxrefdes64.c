/** \file maxrefdes64.c ******************************************************
*
* Project: MAXREFDES64#
* Filename: maxrefdes64.c
* Description: This module contains the Main application for the MAXREFDES64 example program.
*
* Revision History:
*\n 10-07-14 Rev 01.00 MG Initial release.
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
/*!\mainpage Main Page
*
* \section intro_sec Introduction
*
* This is the code documentation for the MAXREFDES64# subsystem reference design.
* \n
* \n The Files page contains the File List page and the Globals page.
* \n
* \n The Globals page contains the Functions, Variables, and Macros sub-pages.
*
* \image html MAXREFDES64_System_Block_Diagram_V10.png "MAXREFDES64# System Block Diagram"
* \n
* \image html MAXREFDES64_Firmware_Flowchart_V10.png "MAXREFDES64# Firmware Flowchart"
*
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "init_config.h"
#include "menu.h"
#include "utilities.h"
#include "max31913.h"
#include <stdio.h>

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAJOR_REVISION 01
#define MINOR_REVISION 00
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * \brief  Main function for MAXREFDES64
	* \par		Details
	*					This function initializes the peripherals and hardware.  
	*					Displays the menu on the terminal program for user control.
  * \param  None
  * \retval Always TRUE
  */
int main(void)
{
	int n_menu_state=MAIN_MENU;	
	uint8_t uch_first_entry = 1; 
	uint8_t uch_input;
	uint16_t u_miso;
	uint8_t u_byte = 0; 
	
	maxim_gpio_init();
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);		// LED_G = 0.
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);		// LED_R = 0.
	maxim_spi_init();
	maxim_uart_init();
	
	// Clear the Screen, and then display the big Maxim banner for about 2 seconds
	maxim_menu_cls();
	maxim_menu_print_maxim_banner_big();
	printf("Maxim MAXREFDES64#\r\n");
	printf("Revision v%d.%d\r\n",MAJOR_REVISION,MINOR_REVISION);

	maxim_delay(2*ONE_SECOND);	//2 second pause
	
	while(1)
	{
		switch(n_menu_state)
		{
			case MAIN_MENU:
				maxim_menu_cls();
				maxim_menu_print_main_menu();
				do{
						if(uch_first_entry)
						{
							if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET)
							{
								maxim_delay(ONE_SECOND);	//1 sec pause
							}
							if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET)
							{
								maxim_delay(ONE_SECOND);	//1 sec pause
								n_menu_state = MAIN_MENU;
								break;
							}
						}
						
						uch_input = getchar(); 

						if(uch_input == '0')
							n_menu_state = CONTINUOUS_READ_MENU;
						else if(uch_input == '1')
							n_menu_state = SINGLE_READ_MENU;
						
				    uch_first_entry=0;		
						printf("%c\r\n",uch_input);
						break;
				}while(1);
				break;

			case CONTINUOUS_READ_MENU:				
				printf("Press ESC to stop Continuous Read Mode\r\n");
				do{
					if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
					{
						uch_input = getchar(); 

						if(uch_input == 0x1B)
						{
							n_menu_state = MAIN_MENU;
							break;
						}
					}
					maxim_delay(ONE_SECOND);
					u_miso = maxim_spi_16bit_transfer(0x00); 	
					u_byte = u_miso>>8; 	
					printf("0X%02X  <", u_byte);
					printf(BYTETOBINARYPATTERN, BYTETOBINARY(u_byte));
					printf(">\r\n");
					printf("Press ESC to stop Continuous Read Mode\r\n");
				}while(1);
				break;

			case SINGLE_READ_MENU:
				printf("Press SPACE BAR to Read the Input\r\n");
				printf("Press ESC to stop Single Read Mode\r\n");
			  do{
					if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
					{
						uch_input = getchar(); 

						if(uch_input == 0x1B)
						{
							n_menu_state = MAIN_MENU;
							break;
						}
						else if(uch_input == 0x20)
						{
							u_miso = maxim_spi_16bit_transfer(0x00); 
							u_byte = u_miso>>8; 	
							printf("0X%02X  <", u_byte);
							printf(BYTETOBINARYPATTERN, BYTETOBINARY(u_byte));
							printf(">\r\n"); 
							printf("Press SPACE BAR to Read the Input\r\n");
							printf("Press ESC to stop Single Read Mode\r\n");
						}
					}
				}while(1);
			
				break;

			default:
				n_menu_state = MAIN_MENU;
				break;
		}
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
