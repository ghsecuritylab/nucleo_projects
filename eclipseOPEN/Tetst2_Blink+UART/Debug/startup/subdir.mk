################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f103xb.s 

OBJS += \
./startup/startup_stm32f103xb.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/inc" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/CMSIS/core" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/CMSIS/device" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/HAL_Driver/Inc/Legacy" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/HAL_Driver/Inc" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/Utilities/STM32F1xx_Nucleo" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


