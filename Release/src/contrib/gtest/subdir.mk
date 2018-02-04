################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/contrib/gtest/gtest-all.cc 

CC_DEPS += \
./src/contrib/gtest/gtest-all.d 

OBJS += \
./src/contrib/gtest/gtest-all.o 


# Each subdirectory must supply rules for building sources it contributes
src/contrib/gtest/%.o: ../src/contrib/gtest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


