################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/platform/hal/Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include src/platform/hal/Core/Startup/subdir.mk
-include src/platform/hal/Core/Src/subdir.mk
-include src/platform/hal/subdir.mk
-include src/middleware/msgbox/subdir.mk
-include src/lib/ubx/utils/subdir.mk
-include src/lib/ubx/subdir.mk
-include src/lib/pid/subdir.mk
-include src/lib/crsf/utils/subdir.mk
-include src/lib/crsf/subdir.mk
-include src/lib/compfilter/subdir.mk
-include src/lib/subdir.mk
-include src/drivers/servo/subdir.mk
-include src/drivers/icm20602/subdir.mk
-include src/drivers/hmc5883l_driver/subdir.mk
-include src/app/modules/attitude/subdir.mk
-include src/app/hardware/tranceiver/rtt24/subdir.mk
-include src/app/hardware/tranceiver/rp3/subdir.mk
-include src/app/hardware/tranceiver/subdir.mk
-include src/app/hardware/imu/icm20602/subdir.mk
-include src/app/hardware/imu/subdir.mk
-include src/app/hardware/gnss/neom8n/subdir.mk
-include src/app/hardware/gnss/subdir.mk
-include src/app/hardware/consur/subdir.mk
-include src/app/hardware/compass/subdir.mk
-include src/app/hardware/battery/subdir.mk
-include src/app/subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := flight_controller
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
flight_controller.elf \

MAP_FILES += \
flight_controller.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
flight_controller.list \

OBJCOPY_HEX += \
flight_controller.hex \

OBJCOPY_BIN += \
flight_controller.bin \


# All Target
all: main-build

# Main-build Target
main-build: flight_controller.elf secondary-outputs

# Tool invocations
flight_controller.elf flight_controller.map: $(OBJS) $(USER_OBJS) C:\Users\unRR\Workspace\STM32CubeIDE\workspace_1.19.0\flight_controller\src\platform\hal\STM32F411CEUX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "flight_controller.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\unRR\Workspace\STM32CubeIDE\workspace_1.19.0\flight_controller\src\platform\hal\STM32F411CEUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="flight_controller.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

flight_controller.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "flight_controller.list"
	@echo 'Finished building: $@'
	@echo ' '

flight_controller.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "flight_controller.hex"
	@echo 'Finished building: $@'
	@echo ' '

flight_controller.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "flight_controller.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout flight_controller.bin flight_controller.elf flight_controller.hex flight_controller.list flight_controller.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
