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
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -ID:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Inc -ID:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/STM32F1xx_HAL_Driver/Inc -ID:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -ID:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -ID:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -ID:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/CMSIS/Device/ST/STM32F1xx/Include -ID:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/CMSIS/Include -ID:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Debug -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


