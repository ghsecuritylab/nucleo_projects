/** \file menu.c ******************************************************
*
* Project: MAXREFDES64#
* Filename: menu.c
* Description:	This module contains all the functions used to
* 							generate the menus and menu options used to run the
*								MAXREFDES64# example firmware.
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

#include "stdio.h"
#include "menu.h"

void maxim_menu_cls()
/**
* \brief       Function to clear the screen via Hyperterminal
*
* \param       None
*
* \retval      None
*/
{
	// The following code will cause a clear screen event on Hyperterminal and many other terminal emulators
	printf("\033[2J");
}


void maxim_menu_print_maxim_banner()
/**
* \brief       Print standard Maxim banner at top of Hyperterminal screen
*
* \param       None
*
* \retval      None
*/
{
	printf( "\r\n");
	printf( "///////////////////////////////////////////////////////////////////\r\n");
	printf( "//                        _____    _____                         //\r\n");
	printf( "//                       /  __ \\  / __  \\                        //\r\n");
	printf( "//                       | |  \\/ / /  | |                        //\r\n");
	printf( "//                       | |    / /   | |                        //\r\n");
	printf( "//                       | |   /  \\   | |                        //\r\n");
	printf( "//                       | |  / /\\ \\  | |                        //\r\n");
	printf( "//                       |_| /_/  \\_\\ |_|                        //\r\n");
	printf( "//                                                               //\r\n");
	printf( "///////////////////////////////////////////////////////////////////\r\n");
	printf("\r\n");
}


void maxim_menu_print_maxim_banner_big()
/**
* \brief       Print large Maxim banner at top of Hyperterminal screen
*
* \param       None
*
* \retval      None
*/
{
	printf( "\r\n");
	printf( "///////////////////////////////////////////////////////////////////\r\n");
	printf( "//                          ###   ###                            //\r\n");
	printf( "//                      ##             ##                        //\r\n");
	printf( "//                                                               //\r\n");
	printf( "//                   ##   _____    _____   ##                    //\r\n");
	printf( "//                       /  __ \\  / __  \\                        //\r\n");
	printf( "//                  ##   | |  \\/ / /  | |   ##                   //\r\n");
	printf( "//                       | |    / /   | |                        //\r\n");
	printf( "//                       | |   /  \\   | |                        //\r\n");
	printf( "//                  ##   | |  / /\\ \\  | |   ##                   //\r\n");
	printf( "//                       |_| /_/  \\_\\ |_|                        //\r\n");
	printf( "//                   ##                    ##                    //\r\n");
	printf( "//                                                               //\r\n");
	printf( "//                      ##             ##                        //\r\n");
	printf( "//                          ###   ###                            //\r\n");
	printf( "//                                                               //\r\n");
	printf( "//                M A X I M   I N T E G R A T E D                //\r\n");
	printf( "//                                                               //\r\n");
	printf( "///////////////////////////////////////////////////////////////////\r\n");

	printf("\r\n\r\n");
}

void maxim_menu_print_prompt()
/**
* \brief       Print a standard prompt for keyboard input "  > "
*
* \param       None
*
* \retval      None
*/
{
	printf("\r\n>> ");
	fflush(stdout);
}


void maxim_menu_print_line()
/**
* \brief       Print one line of dashes across the screen via Hyperterminal
*
* \param       None
*
* \retval      None
*/
{
	printf("----------------------------------------------------\r\n\r\n");
}


uint8_t maxim_menu_retrieve_keypress()
/**
* \brief       Get a single keypress via Hyperterminal.
* \par         Details
*              Returns ascii character corresponding to keypress with some preprocessing.
* \n           Escape sequences (Arrow keys and END) are mapped to decimal 240-244 (see defines)
* \n           Characters "0"-"9" converted to numbers 0-9
* \n           Lower case "a"-"z" converted to uppercase "A"-"Z"
*
* \param		   None
*
* \retval      Character, partially decoded.
*/
{
	uint8_t uch_input;

	uch_input = getchar();

	if(uch_input==27)  // Escape sequence (likely an arrow key)
	{
		uch_input = getchar();
		if(uch_input==91)  // Left bracket (part #2 of the 3 part escape sequence)
		{
			uch_input = getchar();
			if(uch_input==65)
				uch_input = KEYPRESS_ARROW_UP-10;
			if(uch_input==66)
				uch_input = KEYPRESS_ARROW_DOWN-10;
			if(uch_input==67)
				uch_input = KEYPRESS_ARROW_RIGHT-10;
			if(uch_input==68)
				uch_input = KEYPRESS_ARROW_LEFT-10;
			if(uch_input==75)
				uch_input = KEYPRESS_END - 10;
		}
	}
	else if(uch_input>=48 && uch_input <=57) // digits 0..9
		uch_input -=48; // subtract 48 to get the real number converted from the ASCII digit
	else if(uch_input>=97 && uch_input<=122)
		uch_input -=32;  // convert lowercase to uppercase

	return(uch_input);
}

void maxim_menu_print_main_menu()
/**
* \brief       Print the main menu listing choice of module to test
* \par         Details.
*
* \retval      None
*/
{
	maxim_menu_print_maxim_banner();

	printf("Press a number to select read mode\r\n");
	printf("<0> Continuous Read Mode\r\n");
	printf("<1> Single Read Mode\r\n");

	maxim_menu_print_prompt();
}

void maxim_menu_print_configuration_description()
/**
* \brief       Print configuration byte description
* \par         Details.
*
* \retval      None
*/
{
	maxim_menu_cls();
	printf("The configuration byte controls the modes of the output ports\r\n");
	printf("Each bit of the configuration byte sets the output mode for the corresponding \n\routput port\r\n");
	printf("Configuration bit = 0, Output port = High-side mode\r\n");
	printf("Configuration bit = 1, Output port = Push-pull mode\r\n");
	printf("Example:\r\n");
	printf("Configuration byte = 0xF0, output ports 0 to 3 are set to high-side mode,\r\n");
	printf("and output ports 4 to 7 are set to push-pull mode\r\n");
}

void maxim_menu_print_output_data_description()
/**
* \brief       Print output data byte description
* \par         Details.
*
* \retval      None
*/
{
	maxim_menu_cls();
	printf("The data output byte controls the output states\r\n");
	printf("Each bit of the data output byte sets the output state for the corresponding \n\routput port\r\n");
	printf("Output bit = 0, Output = Low\r\n");
	printf("Output bit = 1, Output = High\r\n");
	printf("Example:\r\n");
	printf("Data output byte = 0xF0, outputs 0 to 3 are set to low,\r\n");
	printf("and output 4 to 7 are set to high\r\n");
}
