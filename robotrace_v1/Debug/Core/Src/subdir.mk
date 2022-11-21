################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/AQM0802A.c \
../Core/Src/BNO055.c \
../Core/Src/ICM20608.c \
../Core/Src/INA260.c \
../Core/Src/control.c \
../Core/Src/encoder.c \
../Core/Src/fatfs_sd.c \
../Core/Src/led.c \
../Core/Src/lineSensor.c \
../Core/Src/lineTrace.c \
../Core/Src/main.c \
../Core/Src/markerSensor.c \
../Core/Src/motor.c \
../Core/Src/setup.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/switch.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/timer.c \
../Core/Src/velocityCtrl.c \
../Core/Src/yawRateCtrl.c 

OBJS += \
./Core/Src/AQM0802A.o \
./Core/Src/BNO055.o \
./Core/Src/ICM20608.o \
./Core/Src/INA260.o \
./Core/Src/control.o \
./Core/Src/encoder.o \
./Core/Src/fatfs_sd.o \
./Core/Src/led.o \
./Core/Src/lineSensor.o \
./Core/Src/lineTrace.o \
./Core/Src/main.o \
./Core/Src/markerSensor.o \
./Core/Src/motor.o \
./Core/Src/setup.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/switch.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/timer.o \
./Core/Src/velocityCtrl.o \
./Core/Src/yawRateCtrl.o 

C_DEPS += \
./Core/Src/AQM0802A.d \
./Core/Src/BNO055.d \
./Core/Src/ICM20608.d \
./Core/Src/INA260.d \
./Core/Src/control.d \
./Core/Src/encoder.d \
./Core/Src/fatfs_sd.d \
./Core/Src/led.d \
./Core/Src/lineSensor.d \
./Core/Src/lineTrace.d \
./Core/Src/main.d \
./Core/Src/markerSensor.d \
./Core/Src/motor.d \
./Core/Src/setup.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/switch.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/timer.d \
./Core/Src/velocityCtrl.d \
./Core/Src/yawRateCtrl.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AQM0802A.d ./Core/Src/AQM0802A.o ./Core/Src/AQM0802A.su ./Core/Src/BNO055.d ./Core/Src/BNO055.o ./Core/Src/BNO055.su ./Core/Src/ICM20608.d ./Core/Src/ICM20608.o ./Core/Src/ICM20608.su ./Core/Src/INA260.d ./Core/Src/INA260.o ./Core/Src/INA260.su ./Core/Src/control.d ./Core/Src/control.o ./Core/Src/control.su ./Core/Src/encoder.d ./Core/Src/encoder.o ./Core/Src/encoder.su ./Core/Src/fatfs_sd.d ./Core/Src/fatfs_sd.o ./Core/Src/fatfs_sd.su ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/lineSensor.d ./Core/Src/lineSensor.o ./Core/Src/lineSensor.su ./Core/Src/lineTrace.d ./Core/Src/lineTrace.o ./Core/Src/lineTrace.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/markerSensor.d ./Core/Src/markerSensor.o ./Core/Src/markerSensor.su ./Core/Src/motor.d ./Core/Src/motor.o ./Core/Src/motor.su ./Core/Src/setup.d ./Core/Src/setup.o ./Core/Src/setup.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/switch.d ./Core/Src/switch.o ./Core/Src/switch.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/timer.d ./Core/Src/timer.o ./Core/Src/timer.su ./Core/Src/velocityCtrl.d ./Core/Src/velocityCtrl.o ./Core/Src/velocityCtrl.su ./Core/Src/yawRateCtrl.d ./Core/Src/yawRateCtrl.o ./Core/Src/yawRateCtrl.su

.PHONY: clean-Core-2f-Src

