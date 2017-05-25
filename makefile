#makefile for desolateESC


TARGET ?=WRAITH32_35A

include ./src/TARGETs/$(TARGET)/mksetup.mk


# Compile-time options
OPTIONS		?=

# Debugger optons, must be empty or GDB
DEBUG ?=

# Working directories
ROOT		 = ./
OBJECT_DIR	 = $(ROOT)/obj
BIN_DIR		 = $(ROOT)/obj


# Source files common to all targets
SRC  = $(wildcard ./src/*.c)

#target specific files
SRC  += $(wildcard ./src/TARGETs/$(TARGET)/*.c)

TARGET_MCU ?=STM32F051


ifeq ($(TARGET_MCU),STM32F051)
	MCU_FLAG = STM32F051x8
	LIBSDIR    = ./libraries
	CORELIBDIR = $(LIBSDIR)/CMSIS/Include
	DEVDIR  = $(LIBSDIR)/CMSIS/Device/ST/STM32F0xx
	STMLLDIR    = $(LIBSDIR)/STM32F0xx_ll_Driver
	STMLLSRCDDIR = $(STMLLDIR)/src/
	STMLLINCDDIR = $(STMLLDIR)/inc
	SRC +=  ./startup/startup_stm32f051x8.s
	LD_SCRIPT	 ?= ./linker/STM32F05xxx_FLASH.ld
	
	SRC += $(wildcard ./src/MCUs/STM32F051/*.c)
	SRC += $(wildcard $(STMLLSRCDDIR)*.c)
endif


CC		 = arm-none-eabi-gcc
OBJCOPY		 = arm-none-eabi-objcopy

INCLUDE_DIRS = $(DEVDIR)/Include \
          $(CORELIBDIR) \
          $(STMLLINCDDIR) \
	  ./src/TARGETs/$(TARGET) \
	  ./src/MCUs/$(TARGET_MCU) \
          ./src

ARCH_FLAGS	 = -mthumb -mcpu=cortex-m0
BASE_CFLAGS		 = $(ARCH_FLAGS) \
		   $(addprefix -D,$(OPTIONS)) \
		   $(addprefix -I,$(INCLUDE_DIRS)) \
		   -ffunction-sections \
		   -fdata-sections \
		   -DUSE_FULL_LL_DRIVER \
		   -D$(TARGET_MCU) \
		   -D$(TARGET) \
		   -D$(MCU_FLAG)

ASFLAGS		 = $(ARCH_FLAGS) \
		   -x assembler-with-cpp \
		   $(addprefix -I,$(INCLUDE_DIRS))

LDFLAGS		 = -lm \
		   $(ARCH_FLAGS) \
		   -static \
		   -Wl,-gc-sections \
		   -T$(LD_SCRIPT)



CFLAGS = $(BASE_CFLAGS) \
	-Os

TRGT = arm-none-eabi-

TARGET_HEX	 = $(BIN_DIR)/desolateESC_$(TARGET).hex
TARGET_ELF	 = $(BIN_DIR)/desolateESC_$(TARGET).elf
TARGET_OBJS	 = $(addsuffix .o,$(addprefix $(OBJECT_DIR)/,$(basename $(SRC))))


$(TARGET_HEX): $(TARGET_ELF)
	$(OBJCOPY) -O ihex $< $@
	$(TRGT)size $(TARGET_ELF)
	$(TRGT)size $(TARGET_HEX)

$(TARGET_ELF):  $(TARGET_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	

# Compile
$(OBJECT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(CFLAGS) $<

# Assemble
$(OBJECT_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(ASFLAGS) $< 
$(OBJECT_DIR)/%.o): %.S
	@mkdir -p $(dir $@)
	@echo %% $(notdir $<)
	@$(CC) -c -o $@ $(ASFLAGS) $< 

clean:
	rm -f $(TARGET_HEX) $(TARGET_ELF) $(TARGET_OBJS)
