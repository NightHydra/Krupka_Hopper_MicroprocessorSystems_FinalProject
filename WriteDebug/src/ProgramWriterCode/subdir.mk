################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ProgramWriterCode/write_main.c 

OBJS += \
./src/ProgramWriterCode/write_main.o 

C_DEPS += \
./src/ProgramWriterCode/write_main.d 


# Each subdirectory must supply rules for building sources it contributes
src/ProgramWriterCode/%.o src/ProgramWriterCode/%.su src/ProgramWriterCode/%.cyclo: ../src/ProgramWriterCode/%.c src/ProgramWriterCode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-ProgramWriterCode

clean-src-2f-ProgramWriterCode:
	-$(RM) ./src/ProgramWriterCode/write_main.cyclo ./src/ProgramWriterCode/write_main.d ./src/ProgramWriterCode/write_main.o ./src/ProgramWriterCode/write_main.su

.PHONY: clean-src-2f-ProgramWriterCode

