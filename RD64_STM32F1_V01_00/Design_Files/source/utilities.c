/** \file utilities.c ******************************************************
*
* Project: MAXREFDES64#
* Filename: utilities.c
*	Description:	This module contains a collection of general utility
*								functions which are not specific to any particular
*								module.
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

#include "utilities.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"

void maxim_delay(uint32_t un_delay)
/**
* \brief				Delay function
*
* \param[in]		un_delay	-delay factor
*
* \retval				None
*/
{
	uint32_t un_count;
	for(un_count=0;un_count<un_delay;un_count++);
}

int maxim_get_hex(void)
/**
* \brief       Receive inputs from UART and convert the HEX values to interger
*
* \param       None
*
* \retval      None
*/
{
	char s_string[20];
	char ch_char;
	int i=0;
	
	printf(">> 0x");
	for(i=0; i<20 && ch_char!=0x0D; i++)
	{
		ch_char=getchar();
		printf("%c",ch_char);
		s_string[i]=ch_char;
	}
	s_string[i]='\0';
	return maxim_htoi(s_string);
}

int maxim_htoi(char *ps_str)
/**
* \brief				Convert HEX to interger
*
* \param[in]		ps_str		- a string that contains the hex value in ascii characters
*
* \retval				None
*/
{
    int n_ret=0;
    int n_end=0;
		int i;
    for(i=0;i<100;i++)
    {
        switch(ps_str[i])
        {
            case '0': n_ret=n_ret<<4; n_ret=n_ret+0; break;
            case '1': n_ret=n_ret<<4; n_ret=n_ret+1; break;
            case '2': n_ret=n_ret<<4; n_ret=n_ret+2; break;
            case '3': n_ret=n_ret<<4; n_ret=n_ret+3; break;
            case '4': n_ret=n_ret<<4; n_ret=n_ret+4; break;
            case '5': n_ret=n_ret<<4; n_ret=n_ret+5; break;
            case '6': n_ret=n_ret<<4; n_ret=n_ret+6; break;
            case '7': n_ret=n_ret<<4; n_ret=n_ret+7; break;
            case '8': n_ret=n_ret<<4; n_ret=n_ret+8; break;
            case '9': n_ret=n_ret<<4; n_ret=n_ret+9; break;
            case 'a': n_ret=n_ret<<4; n_ret=n_ret+10; break;
            case 'b': n_ret=n_ret<<4; n_ret=n_ret+11; break;
            case 'c': n_ret=n_ret<<4; n_ret=n_ret+12; break;
            case 'd': n_ret=n_ret<<4; n_ret=n_ret+13; break;
            case 'e': n_ret=n_ret<<4; n_ret=n_ret+14; break;
            case 'f': n_ret=n_ret<<4; n_ret=n_ret+15; break;
            case 'A': n_ret=n_ret<<4; n_ret=n_ret+10; break;
            case 'B': n_ret=n_ret<<4; n_ret=n_ret+11; break;
            case 'C': n_ret=n_ret<<4; n_ret=n_ret+12; break;
            case 'D': n_ret=n_ret<<4; n_ret=n_ret+13; break;
            case 'E': n_ret=n_ret<<4; n_ret=n_ret+14; break;
            case 'F': n_ret=n_ret<<4; n_ret=n_ret+15; break;
            default: n_end=1; break;
        }
        if(n_end==1)
        {
            break;
        }
    }
    return n_ret;
}
