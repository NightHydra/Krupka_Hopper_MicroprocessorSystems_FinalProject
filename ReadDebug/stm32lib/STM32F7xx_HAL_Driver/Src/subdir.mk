################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c 

OBJS += \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.o \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o 

C_DEPS += \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.d \
./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.d 


# Each subdirectory must supply rules for building sources it contributes
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o: C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c stm32lib/STM32F7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-stm32lib-2f-STM32F7xx_HAL_Driver-2f-Src

clean-stm32lib-2f-STM32F7xx_HAL_Driver-2f-Src:
	-$(RM) ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_adc.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_spi.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.su ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.cyclo ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.d ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.o ./stm32lib/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.su

.PHONY: clean-stm32lib-2f-STM32F7xx_HAL_Driver-2f-Src

