################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Windows/StopwatchDLG.c 

OBJS += \
./Src/Windows/StopwatchDLG.o 

C_DEPS += \
./Src/Windows/StopwatchDLG.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Windows/StopwatchDLG.o: ../Src/Windows/StopwatchDLG.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F746xx -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../STemWinLib/Inc -I../Drivers/Drajveri_Ekran_Tac/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/Windows/StopwatchDLG.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

