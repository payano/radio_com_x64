################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Mqtt.cpp \
../src/Mqtt_com.cpp \
../src/Radio_com.cpp \
../src/radio_com_x64.cpp 

C_SRCS += \
../src/af_ieee802154.c 

OBJS += \
./src/Mqtt.o \
./src/Mqtt_com.o \
./src/Radio_com.o \
./src/af_ieee802154.o \
./src/radio_com_x64.o 

CPP_DEPS += \
./src/Mqtt.d \
./src/Mqtt_com.d \
./src/Radio_com.d \
./src/radio_com_x64.d 

C_DEPS += \
./src/af_ieee802154.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


