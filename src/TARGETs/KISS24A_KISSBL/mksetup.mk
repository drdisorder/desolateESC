#starting at 0x800 with vector table in ram
TARGET_MCU = STM32F051
SRC  += $(wildcard ./src/TARGETs/KISS24A/*.c)
INCLUDE_DIRS += ./src/TARGETs/KISS24A
LD_SCRIPT	 = ./src/TARGETs/KISS24A_KISSBL/STM32F051xxx_FLASH_KISSBL.ld