################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/port_app_mapper.c 

OBJS += \
./Drivers/port_app_mapper.o 

C_DEPS += \
./Drivers/port_app_mapper.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o: ../Drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303xE -I"C:/Project/KIP-M5_4_303/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/webserver" -I"C:/Project/KIP-M5_4_303/Drivers/modbus" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/include" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/rtu" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/tcp" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/ascii" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/functions" -I"C:/Project/KIP-M5_4_303/Drivers/modbus/port" -I"C:/Project/KIP-M5_4_303/Drivers/Mercury" -I"C:/Project/KIP-M5_4_303/Drivers/STM32F3xx_HAL_Driver/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Project/KIP-M5_4_303/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Project/KIP-M5_4_303/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Project/KIP-M5_4_303/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Project/KIP-M5_4_303/Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"C:/Project/KIP-M5_4_303/Drivers/CMSIS/Include" -I"C:/Project/KIP-M5_4_303/Drivers/uIP" -I"C:/Project/KIP-M5_4_303/Drivers/enc28j60" -I"C:/Project/KIP-M5_4_303/Drivers/ADS1115" -I"C:/Project/KIP-M5_4_303/Drivers" -I"C:/Project/KIP-M5_4_303/Drivers/Sim800_V2-master"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


