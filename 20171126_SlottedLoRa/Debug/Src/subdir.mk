################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c 

OBJS += \
./Src/main.o 

C_DEPS += \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' -DUSE_BAND_915 -DUSE_MODEM_LORA '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Inc -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Middlewares/Third_Party/FreeRTOS/Source/include -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Drivers/STM32F1xx_HAL_Driver/Inc -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Drivers/CMSIS/Device/ST/STM32F1xx/Include -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Drivers/CMSIS/Include -ID:/lam.hq/UbicomLab/LoRa_Implementation/LoRa_End_Device/20171126_SlottedLoRa/Debug -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


