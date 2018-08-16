################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../framework/DCDC/Source/DCDC.c 

OBJS += \
./framework/DCDC/Source/DCDC.o 

C_DEPS += \
./framework/DCDC/Source/DCDC.d 


# Each subdirectory must supply rules for building sources it contributes
framework/DCDC/Source/%.o: ../framework/DCDC/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKW41Z512CAT4_cm0plus -DCPU_MKW41Z512CAT4 -DDEBUG -DCPU_MKW41Z512VHT4 -DFSL_RTOS_FREE_RTOS -DFRDM_KW41Z -DFREEDOM -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -I../board -I../source -I../ -I../framework/OSAbstraction/Interface -I../framework/common -I../freertos -I../framework/Flash/Internal -I../framework/GPIO -I../framework/Keyboard/Interface -I../framework/TimersManager/Interface -I../framework/TimersManager/Source -I../framework/LED/Interface -I../framework/MemManager/Interface -I../framework/Lists -I../framework/Messaging/Interface -I../framework/LowPower/Interface/MKW41Z -I../framework/LowPower/Source/MKW41Z -I../framework/Panic/Interface -I../framework/RNG/Interface -I../framework/NVM/Interface -I../framework/NVM/Source -I../framework/SerialManager/Interface -I../framework/SerialManager/Source/I2C_Adapter -I../framework/SerialManager/Source/SPI_Adapter -I../framework/SerialManager/Source/UART_Adapter -I../framework/ModuleInfo -I../framework/FunctionLib -I../framework/SecLib -I../bluetooth/host/interface -I../source/common -I../bluetooth/host/config -I../bluetooth/controller/interface -I../bluetooth/hci_transport/interface -I../source/common/gatt_db/macros -I../source/common/gatt_db -I../bluetooth/profiles/battery -I../bluetooth/profiles/device_info -I../bluetooth/profiles/heart_rate -I../framework/MWSCoexistence/Interface -I../drivers -I../CMSIS -I../utilities -I../framework/DCDC/Interface -I../framework/XCVR/MKW41Z4 -O0 -fno-common -g -Wall -c  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin -imacros "C:/Users/dav/Documents/MCUXpressoIDE_10.2.1_795/workspace/frdmkw41z_wireless_examples_bluetooth_heart_rate_sensor_freertos/source/app_preinclude.h" -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


