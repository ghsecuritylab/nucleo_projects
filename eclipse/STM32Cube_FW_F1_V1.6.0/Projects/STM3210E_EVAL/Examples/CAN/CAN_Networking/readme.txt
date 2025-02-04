/**
  @page CAN_Networking CAN Networking example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CAN/CAN_Networking/readme.txt 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    14-April-2017
  * @brief   Description of the CAN Networking example.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example shows how to configure the CAN peripheral to send and receive 
CAN frames in normal mode. 

The sent frames are used to control LEDs by pressing Key push-button.
	
The CAN serial communication link is a bus to which a number of units may be
connected. This number has no theoretical limit. Practically the total number
of units will be limited by delay times and/or electrical loads on the bus line.

This program behaves as follows:
  - After reset, all LEDs are OFF on the N eval boards
  - By Pressing on Key push-button : LED1 turns ON and all other LEDs are OFF, on the N
    eval boards connected to the bus. 
  - Press on Key push-button again to send CAN Frame to command LEDn+1 ON, all other LEDs 
    are OFF on the N eval boards.
    
@note This example is tested with a bus of 3 units. The same program example is 
      loaded in all units to send and receive frames.
@note Any unit in the CAN bus may play the role of sender (by pressing Key push-button)
      or receiver.

  The CAN is configured as follows:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
    - ID Filter  = All identifiers are allowed
    - RTR = Data
    - DLC = 1 byte
    - Data: Led number that should be turned ON

@par Directory contents 

  - CAN/CAN_Networking/Inc/stm32f1xx_hal_conf.h    HAL configuration file
  - CAN/CAN_Networking/Inc/stm32f1xx_it.h          DMA interrupt handlers header file
  - CAN/CAN_Networking/Inc/main.h                  Header for main.c module  
  - CAN/CAN_Networking/Src/stm32f1xx_it.c          DMA interrupt handlers
  - CAN/CAN_Networking/Src/main.c                  Main program
  - CAN/CAN_Networking/Src/stm32f1xx_hal_msp.c     HAL MSP file
  - CAN/CAN_Networking/Src/system_stm32f1xx.c      STM32F1xx system source file
  
@par Hardware and Software environment

  - This example runs on STM32F103xG devices.
  - This example has been tested with STM3210E-EVAL RevD board and can be
    easily tailored to any other supported device and development board.

  - STM3210E-EVAL RevD Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PF.06, PF.07, PF.08
      and PF.09 pins
    - Use Key push-button connected to PG.08
    - Connect a male/male CAN cable between at least 2 EVAL CAN connectors (CN4)
    - You have to configure the jumpers as follows:

  @verbatim
  
       +-------------------------------+
       |   Jumper    |       CAN       |
       +-------------------------------+
       |   JP8       |       1-2       |
       |   JP6       |      fitted     |
       +-------------------------------+
  @endverbatim
  
@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 