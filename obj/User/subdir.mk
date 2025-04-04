################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/button.c \
../User/ch32x035_it.c \
../User/ch32x035_usbfs_device.c \
../User/main.c \
../User/system_ch32x035.c \
../User/usb_desc.c \
../User/usbd_compostie_km.c 

C_DEPS += \
./User/button.d \
./User/ch32x035_it.d \
./User/ch32x035_usbfs_device.d \
./User/main.d \
./User/system_ch32x035.d \
./User/usb_desc.d \
./User/usbd_compostie_km.d 

OBJS += \
./User/button.o \
./User/ch32x035_it.o \
./User/ch32x035_usbfs_device.o \
./User/main.o \
./User/system_ch32x035.o \
./User/usb_desc.o \
./User/usbd_compostie_km.o 



# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	riscv-wch-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"c:/Users/giahu/mounriver-studio-projects/CH32X035F8U6/Debug" -I"c:/Users/giahu/mounriver-studio-projects/CH32X035F8U6/Core" -I"c:/Users/giahu/mounriver-studio-projects/CH32X035F8U6/User" -I"c:/Users/giahu/mounriver-studio-projects/CH32X035F8U6/Peripheral/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
