################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/Graphics/Graphics/GraphObjects/Snake.cpp 

OBJS += \
./Core/Src/Graphics/Graphics/GraphObjects/Snake.o 

CPP_DEPS += \
./Core/Src/Graphics/Graphics/GraphObjects/Snake.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Graphics/Graphics/GraphObjects/%.o Core/Src/Graphics/Graphics/GraphObjects/%.su Core/Src/Graphics/Graphics/GraphObjects/%.cyclo: ../Core/Src/Graphics/Graphics/GraphObjects/%.cpp Core/Src/Graphics/Graphics/GraphObjects/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Graphics-2f-Graphics-2f-GraphObjects

clean-Core-2f-Src-2f-Graphics-2f-Graphics-2f-GraphObjects:
	-$(RM) ./Core/Src/Graphics/Graphics/GraphObjects/Snake.cyclo ./Core/Src/Graphics/Graphics/GraphObjects/Snake.d ./Core/Src/Graphics/Graphics/GraphObjects/Snake.o ./Core/Src/Graphics/Graphics/GraphObjects/Snake.su

.PHONY: clean-Core-2f-Src-2f-Graphics-2f-Graphics-2f-GraphObjects

