################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CarAI.cpp \
../Game.cpp \
../GameObject.cpp \
../PlayerControls.cpp \
../SliderCar.cpp \
../TeapotRacerGL.cpp \
../Weapon.cpp 

OBJS += \
./CarAI.o \
./Game.o \
./GameObject.o \
./PlayerControls.o \
./SliderCar.o \
./TeapotRacerGL.o \
./Weapon.o 

CPP_DEPS += \
./CarAI.d \
./Game.d \
./GameObject.d \
./PlayerControls.d \
./SliderCar.d \
./TeapotRacerGL.d \
./Weapon.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


