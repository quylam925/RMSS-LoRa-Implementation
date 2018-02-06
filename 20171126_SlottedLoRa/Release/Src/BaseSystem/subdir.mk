################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/BaseSystem/delay.c \
../Src/BaseSystem/gpio.c \
../Src/BaseSystem/rtc.c \
../Src/BaseSystem/spi.c \
../Src/BaseSystem/stm32f1xx_hal_msp.c \
../Src/BaseSystem/stm32f1xx_it.c \
../Src/BaseSystem/system_stm32f1xx.c \
../Src/BaseSystem/timer.c \
../Src/BaseSystem/usb_device.c \
../Src/BaseSystem/usbd_cdc_if.c \
../Src/BaseSystem/usbd_conf.c \
../Src/BaseSystem/usbd_desc.c \
../Src/BaseSystem/utilities.c 

OBJS += \
./Src/BaseSystem/delay.o \
./Src/BaseSystem/gpio.o \
./Src/BaseSystem/rtc.o \
./Src/BaseSystem/spi.o \
./Src/BaseSystem/stm32f1xx_hal_msp.o \
./Src/BaseSystem/stm32f1xx_it.o \
./Src/BaseSystem/system_stm32f1xx.o \
./Src/BaseSystem/timer.o \
./Src/BaseSystem/usb_device.o \
./Src/BaseSystem/usbd_cdc_if.o \
./Src/BaseSystem/usbd_conf.o \
./Src/BaseSystem/usbd_desc.o \
./Src/BaseSystem/utilities.o 

C_DEPS += \
./Src/BaseSystem/delay.d \
./Src/BaseSystem/gpio.d \
./Src/BaseSystem/rtc.d \
./Src/BaseSystem/spi.d \
./Src/BaseSystem/stm32f1xx_hal_msp.d \
./Src/BaseSystem/stm32f1xx_it.d \
./Src/BaseSystem/system_stm32f1xx.d \
./Src/BaseSystem/timer.d \
./Src/BaseSystem/usb_device.d \
./Src/BaseSystem/usbd_cdc_if.d \
./Src/BaseSystem/usbd_conf.d \
./Src/BaseSystem/usbd_desc.d \
./Src/BaseSystem/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
Src/BaseSystem/%.o: ../Src/BaseSystem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Inc" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/lam.hq/UbicomLab/LoRa_Implementation/v2.01/LoRa_End_Device_STM/Drivers/CMSIS/Include"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


