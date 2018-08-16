################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: frdmkw41z_driver_examples_gpio_led_output.axf

# Tool invocations
frdmkw41z_driver_examples_gpio_led_output.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="frdmkw41z_driver_examples_gpio_led_output.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0plus -mthumb -T frdmkw41z_driver_examples_gpio_led_output_Debug.ld -o "frdmkw41z_driver_examples_gpio_led_output.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) frdmkw41z_driver_examples_gpio_led_output.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "frdmkw41z_driver_examples_gpio_led_output.axf"; # arm-none-eabi-objcopy -v -O binary "frdmkw41z_driver_examples_gpio_led_output.axf" "frdmkw41z_driver_examples_gpio_led_output.bin" ; # checksum -p MKW41Z512xxx4 -d "frdmkw41z_driver_examples_gpio_led_output.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
