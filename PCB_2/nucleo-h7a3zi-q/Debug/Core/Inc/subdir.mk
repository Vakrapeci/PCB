################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Inc/Fields.cpp \
../Core/Inc/MessageInterface.cpp \
../Core/Inc/ReadBufferSection.cpp 

OBJS += \
./Core/Inc/Fields.o \
./Core/Inc/MessageInterface.o \
./Core/Inc/ReadBufferSection.o 

CPP_DEPS += \
./Core/Inc/Fields.d \
./Core/Inc/MessageInterface.d \
./Core/Inc/ReadBufferSection.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/%.o Core/Inc/%.su Core/Inc/%.cyclo: ../Core/Inc/%.cpp Core/Inc/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H7A3xxQ -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc

clean-Core-2f-Inc:
	-$(RM) ./Core/Inc/Fields.cyclo ./Core/Inc/Fields.d ./Core/Inc/Fields.o ./Core/Inc/Fields.su ./Core/Inc/MessageInterface.cyclo ./Core/Inc/MessageInterface.d ./Core/Inc/MessageInterface.o ./Core/Inc/MessageInterface.su ./Core/Inc/ReadBufferSection.cyclo ./Core/Inc/ReadBufferSection.d ./Core/Inc/ReadBufferSection.o ./Core/Inc/ReadBufferSection.su

.PHONY: clean-Core-2f-Inc

