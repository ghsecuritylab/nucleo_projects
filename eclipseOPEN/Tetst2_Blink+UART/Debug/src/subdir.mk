################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f1xx_hal_msp.c \
../src/stm32f1xx_it.c \
../src/syscalls.c \
../src/system_stm32f1xx.c 

OBJS += \
./src/main.o \
./src/stm32f1xx_hal_msp.o \
./src/stm32f1xx_it.o \
./src/syscalls.o \
./src/system_stm32f1xx.o 

C_DEPS += \
./src/main.d \
./src/stm32f1xx_hal_msp.d \
./src/stm32f1xx_it.d \
./src/syscalls.d \
./src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -DUSE_RTOS_SYSTICK -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/inc" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/CMSIS/core" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/CMSIS/device" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/HAL_Driver/Inc/Legacy" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/HAL_Driver/Inc" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"C:/STM32Toolchain/projects/eclipseOPEN/Tetst2_Blink+UART/Utilities/STM32F1xx_Nucleo" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


