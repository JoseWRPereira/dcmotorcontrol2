################################################################################
# Automatically-generated file by eclipse cpp mars2 and 
# modified by José W. R. Pereira
#   
#	Organization of the project:
#	  ~/<Project_directory>
#	  ~/<Project_directory>/debug/
#	  ~/<Project_directory>/src/
#
################################################################################

PROJECT_NAME := alvorada

SERIAL:= /dev/ttyACM0

MAKE_ADDR := $(PWD)

LINKER_ADDR  := $(MAKE_ADDR)/../src/linker.ld

RM := rm -rf

####################################### sources.mk
ELF_SRCS := 
O_SRCS := 
C_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS :=  
SECONDARY_SIZE := 
OBJS := 
C_DEPS := 
ASM_DEPS := 
SECONDARY_FLASH := 
S_UPPER_DEPS := 

# Every subdirectory with source files must be described here
SUBDIRS := \
src \

####################################### 

####################################### src/subdir.mk
# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/startup_gcc.c \
../src/pll.c \
../src/io.c \
../src/uart.c \
../src/systick.c \
../src/fifo.c \
../src/pwm.c \
../src/timer.c \
../src/main.c \

#../src/main.c \
../src/swleds.c \
../src/uart2.c \
../src/controlador.c

OBJS += \
./src/startup_gcc.o \
./src/pll.o \
./src/io.o \
./src/uart.o \
./src/systick.o \
./src/fifo.o \
./src/pwm.o \
./src/timer.o \
./src/main.o \

#./src/main.o \
./src/swleds.o \
./src/uart2.o \
./src/controlador.o

C_DEPS += \
./src/startup_gcc.d \
./src/pll.d \
./src/io.d \
./src/uart.d \
./src/systick.d \
./src/fifo.d \
./src/pwm.d \
./src/timer.d \
./src/main.d \

#./src/main.d \
./src/swleds.d \
./src/uart2.d \
./src/controlador.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DARM_MATH_CM4 -DPART_TM4C123GH6PM -DTARGET_IS_TM4C123_RB1 -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

#######################################

####################################### objects.mk
USER_OBJS :=

LIBS :=

####################################### 


# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
$(PROJECT_NAME).hex \

SECONDARY_SIZE += \
$(PROJECT_NAME).siz \


# All Target
all: $(PROJECT_NAME).elf secondary-outputs

# Tool invocations
$(PROJECT_NAME).elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "$(LINKER_ADDR)" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"$(PROJECT_NAME).map" --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group -o "$(PROJECT_NAME).elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf post-build
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "$(PROJECT_NAME).elf"  "$(PROJECT_NAME).hex"
	@echo 'Finished building: $@'
	@echo ' '

$(PROJECT_NAME).siz: $(PROJECT_NAME).elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "$(PROJECT_NAME).elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(S_UPPER_DEPS) $(PROJECT_NAME).*
	-@echo ' '

post-build:
	-arm-none-eabi-objcopy -O binary $(PROJECT_NAME).elf $(PROJECT_NAME).bin
	-@echo ' postBuild'

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

usb: tty
	ls /dev/ttyACM*
tty:
	dmesg | grep tty

serial:
	minicom -D $(SERIAL)

grava: 
	lm4flash -S $(SERIAL) $(PROJECT_NAME).bin

