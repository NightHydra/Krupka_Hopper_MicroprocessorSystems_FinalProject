################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cartridge_object.c \
../src/init.c \
../src/spi_flash_interface.c \
../src/uart.c 

OBJS += \
./src/cartridge_object.o \
./src/init.o \
./src/spi_flash_interface.o \
./src/uart.o 

C_DEPS += \
./src/cartridge_object.d \
./src/init.d \
./src/spi_flash_interface.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DUSE_STM32F769I_DISCO -c -I"C:/MicroprocessorSystems/FinalTemplate/inc" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Include" -I"C:/MicroprocessorSystems/stm32lib/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/MicroprocessorSystems/stm32lib/STM32F7xx_HAL_Driver/Inc" -Og -ffunction-sections -Wall -Wdouble-promotion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/cartridge_object.cyclo ./src/cartridge_object.d ./src/cartridge_object.o ./src/cartridge_object.su ./src/init.cyclo ./src/init.d ./src/init.o ./src/init.su ./src/spi_flash_interface.cyclo ./src/spi_flash_interface.d ./src/spi_flash_interface.o ./src/spi_flash_interface.su ./src/uart.cyclo ./src/uart.d ./src/uart.o ./src/uart.su

.PHONY: clean-src

