################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Sim800_V2-master/Bluetooth.c \
../Drivers/Sim800_V2-master/BluetoothUser.c \
../Drivers/Sim800_V2-master/GPRS.c \
../Drivers/Sim800_V2-master/GPRSUser.c \
../Drivers/Sim800_V2-master/Gsm.c \
../Drivers/Sim800_V2-master/GsmUser.c \
../Drivers/Sim800_V2-master/Sim80x.c \
../Drivers/Sim800_V2-master/Sim80xUser.c 

OBJS += \
./Drivers/Sim800_V2-master/Bluetooth.o \
./Drivers/Sim800_V2-master/BluetoothUser.o \
./Drivers/Sim800_V2-master/GPRS.o \
./Drivers/Sim800_V2-master/GPRSUser.o \
./Drivers/Sim800_V2-master/Gsm.o \
./Drivers/Sim800_V2-master/GsmUser.o \
./Drivers/Sim800_V2-master/Sim80x.o \
./Drivers/Sim800_V2-master/Sim80xUser.o 

C_DEPS += \
./Drivers/Sim800_V2-master/Bluetooth.d \
./Drivers/Sim800_V2-master/BluetoothUser.d \
./Drivers/Sim800_V2-master/GPRS.d \
./Drivers/Sim800_V2-master/GPRSUser.d \
./Drivers/Sim800_V2-master/Gsm.d \
./Drivers/Sim800_V2-master/GsmUser.d \
./Drivers/Sim800_V2-master/Sim80x.d \
./Drivers/Sim800_V2-master/Sim80xUser.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Sim800_V2-master/%.o: ../Drivers/Sim800_V2-master/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303xE -I"C:/Project/KIP-M5_4_303/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/webserver" -I"C:/Project/KIP-M5_4_303/Drivers/modbus" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/include" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/rtu" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/tcp" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/ascii" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/functions" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/port" -I"C:/Project/KIP-M5_4_303/Drivers/Mercury" -I"C:/Project/KIP-M5_4_303/Drivers/STM32F3xx_HAL_Driver/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Project/KIP-M5_4_303/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Project/KIP-M5_4_303/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"C:/Project/KIP-M5_4_303/Drivers/CMSIS/Include" -I"C:/Project/KIP-M5_4_303/Drivers/uIP" -I"C:/Project/KIP-M5_4_303/Drivers/enc28j60" -I"C:/Project/KIP-M5_4_303/Drivers/ADS1115" -I"C:/Project/KIP-M5_4_303/Drivers" -I"C:/Project/KIP-M5_4_303/Drivers/Sim800_V2-master"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


