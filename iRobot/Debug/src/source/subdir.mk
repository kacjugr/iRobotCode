################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/source/AStar.cpp \
../src/source/GameMap.cpp \
../src/source/iRobot.cpp 

OBJS += \
./src/source/AStar.o \
./src/source/GameMap.o \
./src/source/iRobot.o 

CPP_DEPS += \
./src/source/AStar.d \
./src/source/GameMap.d \
./src/source/iRobot.d 


# Each subdirectory must supply rules for building sources it contributes
src/source/%.o: ../src/source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


