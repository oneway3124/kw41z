################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/common/nwk_ip/shell_ip/subdir.mk
-include source/common/nwk_ip/app_sockets/subdir.mk
-include source/common/nwk_ip/app_echo_udp/subdir.mk
-include source/common/nwk_ip/app_coap/subdir.mk
-include source/common/nwk_ip/subdir.mk
-include source/common/gatt_db/subdir.mk
-include source/common/subdir.mk
-include source/subdir.mk
-include nwk_ip/base/ip_media_if/subdir.mk
-include freertos/subdir.mk
-include framework/XCVR/MKW41Z4/cfgs_kw4x_3x_2x/subdir.mk
-include framework/XCVR/MKW41Z4/subdir.mk
-include framework/TimersManager/Source/subdir.mk
-include framework/Shell/Source/subdir.mk
-include framework/SerialManager/Source/UART_Adapter/subdir.mk
-include framework/SerialManager/Source/SPI_Adapter/subdir.mk
-include framework/SerialManager/Source/I2C_Adapter/subdir.mk
-include framework/SerialManager/Source/subdir.mk
-include framework/SecLib/subdir.mk
-include framework/Reset/subdir.mk
-include framework/RNG/Source/subdir.mk
-include framework/Panic/Source/subdir.mk
-include framework/OSAbstraction/Source/subdir.mk
-include framework/NVM/Source/subdir.mk
-include framework/ModuleInfo/subdir.mk
-include framework/Messaging/Source/subdir.mk
-include framework/MemManager/Source/subdir.mk
-include framework/MWSCoexistence/Source/subdir.mk
-include framework/Lists/subdir.mk
-include framework/LED/Source/subdir.mk
-include framework/Keyboard/Source/subdir.mk
-include framework/GPIO/subdir.mk
-include framework/FunctionLib/subdir.mk
-include framework/Flash/Internal/subdir.mk
-include framework/DCDC/Source/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include bluetooth/profiles/ipsp/subdir.mk
-include bluetooth/host/config/subdir.mk
-include bluetooth/hci_transport/source/subdir.mk
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
all: frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.axf

# Tool invocations
frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -L"C:\Users\dav\Documents\MCUXpressoIDE_10.2.1_795\workspace\frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos\libs" -Xlinker -Map="frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0plus -mthumb -T MKW41Z512xxx4_connectivity.ld -L ../source -o "frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.axf"; # arm-none-eabi-objcopy -v -O binary "frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.axf" "frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.bin" ; # checksum -p MKW41Z512xxx4 -d "frdmkw41z_wireless_examples_bluetooth_ipv6_router_freertos.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
