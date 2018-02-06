################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/network/LoRa/aec_cmac.c \
../Src/network/LoRa/aes.c \
../Src/network/LoRa/crypto.c \
../Src/network/LoRa/lora_mac.c \
../Src/network/LoRa/sx1276-ubiboard.c \
../Src/network/LoRa/sx1276.c 

OBJS += \
./Src/network/LoRa/aec_cmac.o \
./Src/network/LoRa/aes.o \
./Src/network/LoRa/crypto.o \
./Src/network/LoRa/lora_mac.o \
./Src/network/LoRa/sx1276-ubiboard.o \
./Src/network/LoRa/sx1276.o 

C_DEPS += \
./Src/network/LoRa/aec_cmac.d \
./Src/network/LoRa/aes.d \
./Src/network/LoRa/crypto.d \
./Src/network/LoRa/lora_mac.d \
./Src/network/LoRa/sx1276-ubiboard.d \
./Src/network/LoRa/sx1276.d 


# Each subdirectory must supply rules for building sources it contributes
Src/network/LoRa/%.o: ../Src/network/LoRa/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Inc" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/CMSIS/Include"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


