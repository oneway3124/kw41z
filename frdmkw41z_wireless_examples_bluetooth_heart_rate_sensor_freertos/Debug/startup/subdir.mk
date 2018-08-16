################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_MKW41Z4.S 

OBJS += \
./startup/startup_MKW41Z4.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__ -I../board -I../source -I../ -I../framework/OSAbstraction/Interface -I../framework/common -I../freertos -I../framework/Flash/Internal -I../framework/GPIO -I../framework/Keyboard/Interface -I../framework/TimersManager/Interface -I../framework/TimersManager/Source -I../framework/LED/Interface -I../framework/MemManager/Interface -I../framework/Lists -I../framework/Messaging/Interface -I../framework/LowPower/Interface/MKW41Z -I../framework/LowPower/Source/MKW41Z -I../framework/Panic/Interface -I../framework/RNG/Interface -I../framework/NVM/Interface -I../framework/NVM/Source -I../framework/SerialManager/Interface -I../framework/SerialManager/Source/I2C_Adapter -I../framework/SerialManager/Source/SPI_Adapter -I../framework/SerialManager/Source/UART_Adapter -I../framework/ModuleInfo -I../framework/FunctionLib -I../framework/SecLib -I../bluetooth/host/interface -I../source/common -I../bluetooth/host/config -I../bluetooth/controller/interface -I../bluetooth/hci_transport/interface -I../source/common/gatt_db/macros -I../source/common/gatt_db -I../bluetooth/profiles/battery -I../bluetooth/profiles/device_info -I../bluetooth/profiles/heart_rate -I../framework/MWSCoexistence/Interface -I../drivers -I../CMSIS -I../utilities -I../framework/DCDC/Interface -I../framework/XCVR/MKW41Z4 -g3 -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


