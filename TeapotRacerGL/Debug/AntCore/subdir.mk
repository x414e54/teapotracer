################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AntCore/AntCamera.cpp \
../AntCore/AntGUI.cpp \
../AntCore/AntSettings.cpp \
../AntCore/AntWrap.cpp \
../AntCore/GLRenderer.cpp \
../AntCore/PointMass.cpp 

OBJS += \
./AntCore/AntCamera.o \
./AntCore/AntGUI.o \
./AntCore/AntSettings.o \
./AntCore/AntWrap.o \
./AntCore/GLRenderer.o \
./AntCore/PointMass.o 

CPP_DEPS += \
./AntCore/AntCamera.d \
./AntCore/AntGUI.d \
./AntCore/AntSettings.d \
./AntCore/AntWrap.d \
./AntCore/GLRenderer.d \
./AntCore/PointMass.d 


# Each subdirectory must supply rules for building sources it contributes
AntCore/%.o: ../AntCore/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


