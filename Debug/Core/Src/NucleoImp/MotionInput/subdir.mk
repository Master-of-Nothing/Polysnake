################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/NucleoImp/MotionInput/MPU6050MotionInput.cpp 

C_SRCS += \
../Core/Src/NucleoImp/MotionInput/mpu6050.c 

C_DEPS += \
./Core/Src/NucleoImp/MotionInput/mpu6050.d 

OBJS += \
./Core/Src/NucleoImp/MotionInput/MPU6050MotionInput.o \
./Core/Src/NucleoImp/MotionInput/mpu6050.o 

CPP_DEPS += \
./Core/Src/NucleoImp/MotionInput/MPU6050MotionInput.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/NucleoImp/MotionInput/%.o Core/Src/NucleoImp/MotionInput/%.su Core/Src/NucleoImp/MotionInput/%.cyclo: ../Core/Src/NucleoImp/MotionInput/%.cpp Core/Src/NucleoImp/MotionInput/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/NucleoImp/MotionInput/%.o Core/Src/NucleoImp/MotionInput/%.su Core/Src/NucleoImp/MotionInput/%.cyclo: ../Core/Src/NucleoImp/MotionInput/%.c Core/Src/NucleoImp/MotionInput/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-NucleoImp-2f-MotionInput

clean-Core-2f-Src-2f-NucleoImp-2f-MotionInput:
	-$(RM) ./Core/Src/NucleoImp/MotionInput/MPU6050MotionInput.cyclo ./Core/Src/NucleoImp/MotionInput/MPU6050MotionInput.d ./Core/Src/NucleoImp/MotionInput/MPU6050MotionInput.o ./Core/Src/NucleoImp/MotionInput/MPU6050MotionInput.su ./Core/Src/NucleoImp/MotionInput/mpu6050.cyclo ./Core/Src/NucleoImp/MotionInput/mpu6050.d ./Core/Src/NucleoImp/MotionInput/mpu6050.o ./Core/Src/NucleoImp/MotionInput/mpu6050.su

.PHONY: clean-Core-2f-Src-2f-NucleoImp-2f-MotionInput

