################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/NucleoImp/SerialCom/COBS.cpp \
../Core/Src/NucleoImp/SerialCom/UartSnakeManager.cpp 

OBJS += \
./Core/Src/NucleoImp/SerialCom/COBS.o \
./Core/Src/NucleoImp/SerialCom/UartSnakeManager.o 

CPP_DEPS += \
./Core/Src/NucleoImp/SerialCom/COBS.d \
./Core/Src/NucleoImp/SerialCom/UartSnakeManager.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/NucleoImp/SerialCom/%.o Core/Src/NucleoImp/SerialCom/%.su Core/Src/NucleoImp/SerialCom/%.cyclo: ../Core/Src/NucleoImp/SerialCom/%.cpp Core/Src/NucleoImp/SerialCom/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-NucleoImp-2f-SerialCom

clean-Core-2f-Src-2f-NucleoImp-2f-SerialCom:
	-$(RM) ./Core/Src/NucleoImp/SerialCom/COBS.cyclo ./Core/Src/NucleoImp/SerialCom/COBS.d ./Core/Src/NucleoImp/SerialCom/COBS.o ./Core/Src/NucleoImp/SerialCom/COBS.su ./Core/Src/NucleoImp/SerialCom/UartSnakeManager.cyclo ./Core/Src/NucleoImp/SerialCom/UartSnakeManager.d ./Core/Src/NucleoImp/SerialCom/UartSnakeManager.o ./Core/Src/NucleoImp/SerialCom/UartSnakeManager.su

.PHONY: clean-Core-2f-Src-2f-NucleoImp-2f-SerialCom

