################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drivers/hmc5883l_driver/hmc5883l_driver.c 

OBJS += \
./src/drivers/hmc5883l_driver/hmc5883l_driver.o 

C_DEPS += \
./src/drivers/hmc5883l_driver/hmc5883l_driver.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/hmc5883l_driver/%.o src/drivers/hmc5883l_driver/%.su src/drivers/hmc5883l_driver/%.cyclo: ../src/drivers/hmc5883l_driver/%.c src/drivers/hmc5883l_driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F411xE -c -I"C:/Users/unRR/Workspace/STM32CubeIDE/workspace_1.19.0/flight_controller/src/platform/hal" -I"C:/Users/unRR/Workspace/STM32CubeIDE/workspace_1.19.0/flight_controller/src/platform/hal/Core/Inc" -I"C:/Users/unRR/Workspace/STM32CubeIDE/workspace_1.19.0/flight_controller/src/platform/hal/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/unRR/Workspace/STM32CubeIDE/workspace_1.19.0/flight_controller/src/platform/hal/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/unRR/Workspace/STM32CubeIDE/workspace_1.19.0/flight_controller/src/platform/hal/Drivers/CMSIS/Include" -I"C:/Users/unRR/Workspace/STM32CubeIDE/workspace_1.19.0/flight_controller/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-drivers-2f-hmc5883l_driver

clean-src-2f-drivers-2f-hmc5883l_driver:
	-$(RM) ./src/drivers/hmc5883l_driver/hmc5883l_driver.cyclo ./src/drivers/hmc5883l_driver/hmc5883l_driver.d ./src/drivers/hmc5883l_driver/hmc5883l_driver.o ./src/drivers/hmc5883l_driver/hmc5883l_driver.su

.PHONY: clean-src-2f-drivers-2f-hmc5883l_driver

