################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/Windows/subdir.mk
-include Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Drivers/Drajveri_Ekran_Tac/Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Display_and_Touch_Screen.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Display_and_Touch_Screen.list \

OBJCOPY_HEX += \
Display_and_Touch_Screen.hex \


# All Target
all: Display_and_Touch_Screen.elf secondary-outputs

# Tool invocations
Display_and_Touch_Screen.elf: $(OBJS) $(USER_OBJS) D:\displej\ 4\Display_and_Touch_Screen\STM32F746ZGTX_FLASH.ld
	arm-none-eabi-gcc -o "Display_and_Touch_Screen.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"D:\displej 4\Display_and_Touch_Screen\STM32F746ZGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Display_and_Touch_Screen.map" -Wl,--gc-sections -static -L../STemWinLib/Lib --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Display_and_Touch_Screen.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Display_and_Touch_Screen.list"
	@echo 'Finished building: $@'
	@echo ' '

Display_and_Touch_Screen.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Display_and_Touch_Screen.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
