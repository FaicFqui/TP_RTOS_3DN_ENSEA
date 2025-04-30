################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drv_uart/drv_uart.c 

OBJS += \
./drv_uart/drv_uart.o 

C_DEPS += \
./drv_uart/drv_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drv_uart/%.o drv_uart/%.su drv_uart/%.cyclo: ../drv_uart/%.c drv_uart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"C:/Users/faiss/Desktop/TP_RTOS_3DN_v1/drv_uart" -I"C:/Users/faiss/Desktop/TP_RTOS_3DN_v1/shell" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drv_uart

clean-drv_uart:
	-$(RM) ./drv_uart/drv_uart.cyclo ./drv_uart/drv_uart.d ./drv_uart/drv_uart.o ./drv_uart/drv_uart.su

.PHONY: clean-drv_uart

