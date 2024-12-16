################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SamplePrograms/Simple_Test/ApplicationMain.c 

OBJS += \
./src/SamplePrograms/Simple_Test/ApplicationMain.o 

C_DEPS += \
./src/SamplePrograms/Simple_Test/ApplicationMain.d 


# Each subdirectory must supply rules for building sources it contributes
src/SamplePrograms/Simple_Test/%.o src/SamplePrograms/Simple_Test/%.su src/SamplePrograms/Simple_Test/%.cyclo: ../src/SamplePrograms/Simple_Test/%.c src/SamplePrograms/Simple_Test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-SamplePrograms-2f-Simple_Test

clean-src-2f-SamplePrograms-2f-Simple_Test:
	-$(RM) ./src/SamplePrograms/Simple_Test/ApplicationMain.cyclo ./src/SamplePrograms/Simple_Test/ApplicationMain.d ./src/SamplePrograms/Simple_Test/ApplicationMain.o ./src/SamplePrograms/Simple_Test/ApplicationMain.su

.PHONY: clean-src-2f-SamplePrograms-2f-Simple_Test

