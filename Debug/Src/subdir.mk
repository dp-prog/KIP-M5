################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/crc.c \
../Src/dma.c \
../Src/freertos.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/iwdg.c \
../Src/main.c \
../Src/rtc.c \
../Src/spi.c \
../Src/stm32f3xx_hal_msp.c \
../Src/stm32f3xx_hal_timebase_tim.c \
../Src/stm32f3xx_it.c \
../Src/sys.c \
../Src/syscalls.c \
../Src/system_stm32f3xx.c \
../Src/tim.c \
../Src/usart.c \
../Src/usb_device.c \
../Src/usbd_cdc_if.c \
../Src/usbd_conf.c \
../Src/usbd_desc.c 

OBJS += \
./Src/adc.o \
./Src/crc.o \
./Src/dma.o \
./Src/freertos.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/iwdg.o \
./Src/main.o \
./Src/rtc.o \
./Src/spi.o \
./Src/stm32f3xx_hal_msp.o \
./Src/stm32f3xx_hal_timebase_tim.o \
./Src/stm32f3xx_it.o \
./Src/sys.o \
./Src/syscalls.o \
./Src/system_stm32f3xx.o \
./Src/tim.o \
./Src/usart.o \
./Src/usb_device.o \
./Src/usbd_cdc_if.o \
./Src/usbd_conf.o \
./Src/usbd_desc.o 

C_DEPS += \
./Src/adc.d \
./Src/crc.d \
./Src/dma.d \
./Src/freertos.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/iwdg.d \
./Src/main.d \
./Src/rtc.d \
./Src/spi.d \
./Src/stm32f3xx_hal_msp.d \
./Src/stm32f3xx_hal_timebase_tim.d \
./Src/stm32f3xx_it.d \
./Src/sys.d \
./Src/syscalls.d \
./Src/system_stm32f3xx.d \
./Src/tim.d \
./Src/usart.d \
./Src/usb_device.d \
./Src/usbd_cdc_if.d \
./Src/usbd_conf.d \
./Src/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303xE -I"C:/Project/KIP-M5_4_303/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/webserver" -I"C:/Project/KIP-M5_4_303/Drivers/modbus" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/include" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/rtu" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/tcp" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/ascii" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/functions" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/port" -I"C:/Project/KIP-M5_4_303/Drivers/Mercury" -I"C:/Project/KIP-M5_4_303/Drivers/STM32F3xx_HAL_Driver/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Project/KIP-M5_4_303/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Project/KIP-M5_4_303/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"C:/Project/KIP-M5_4_303/Drivers/CMSIS/Include" -I"C:/Project/KIP-M5_4_303/Drivers/uIP" -I"C:/Project/KIP-M5_4_303/Drivers/enc28j60" -I"C:/Project/KIP-M5_4_303/Drivers/ADS1115" -I"C:/Project/KIP-M5_4_303/Drivers" -I"C:/Project/KIP-M5_4_303/Drivers/Sim800_V2-master"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


