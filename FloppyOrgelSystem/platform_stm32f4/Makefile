BINPATH=/usr/bin

CC=$(BINPATH)/arm-none-eabi-gcc
AR=$(BINPATH)/arm-none-eabi-ar
LD=$(BINPATH)/arm-none-eabi-ld
OBJCOPY=$(BINPATH)/arm-none-eabi-objcopy
SIZE=$(BINPATH)/arm-none-eabi-size

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

vpath %.c . src lib/src/stdperiph \
	drivers/LCD drivers/NesGamePad drivers/Filesystem drivers/Filesystem/fatfs \
	drivers/Filesystem/fatfs/drivers drivers/Filesystem/fatfs/option \
	../common ../common/AsciiLib ../common/canvas ../common/states ../common/states/playback \
    ../hal

vpath %.a lib

CFLAGS  = -std=gnu99 -g -O2 -Wall -Tstm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4
CFLAGS += -Iinc -Ilib -Ilib/inc
CFLAGS += -Ilib/inc/core -Ilib/inc/stdperiph
CFLAGS += -Idrivers/LCD -Idrivers/NesGamePad
CFLAGS += -Idrivers
CFLAGS += -Idrivers/Filesystem -Idrivers/Filesystem/fatfs -Idrivers/Filesystem/fatfs/drivers
CFLAGS += -I../common -I../common/AsciiLib -I../common/canvas -I../common/states -I../common/states/playback
CFLAGS += -I../hal
CFLAGS += -ffreestanding # -nostdlib # nostdlib parameter should be set!?

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
#CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
else
CFLAGS += -msoft-float
endif

###################################################

OBJDIR=obj
BINDIR=bin
MAIN_SRCS  = main.c stm32f4xx_it.c system_stm32f4xx.c syscalls.c utils.c \
	lib/startup_stm32f4xx.s # add startup file to build

LIB_SRCS = misc.c stm32f4xx_dma.c stm32f4xx_rcc.c stm32f4xx_adc.c \
	stm32f4xx_exti.c stm32f4xx_rng.c stm32f4xx_can.c stm32f4xx_flash.c \
	stm32f4xx_rtc.c stm32f4xx_crc.c stm32f4xx_fsmc.c stm32f4xx_sdio.c \
	stm32f4xx_cryp_aes.c stm32f4xx_gpio.c stm32f4xx_spi.c \
	stm32f4xx_cryp.c stm32f4xx_hash.c stm32f4xx_syscfg.c \
	stm32f4xx_cryp_des.c stm32f4xx_hash_md5.c stm32f4xx_tim.c \
	stm32f4xx_cryp_tdes.c stm32f4xx_hash_sha1.c  stm32f4xx_usart.c \
	stm32f4xx_dac.c stm32f4xx_i2c.c stm32f4xx_wwdg.c \
	stm32f4xx_dbgmcu.c stm32f4xx_iwdg.c \
	stm32f4xx_dcmi.c stm32f4xx_pwr.c

DRIVERS_SRCS = SSD1289.c NesGamePad.c \
	tm_stm32f4_delay.c tm_stm32f4_fatfs.c tm_stm32f4_gpio.c tm_stm32f4_timer_properties.c \
	diskio.c ff.c fatfs_sd_sdio.c ccsbcs.c syscall.c

COMMON_SRCS = main.c common_main.c InputDevice.c LockFreeFifo.c SlotBasedMenu.c StackBasedFsm.c \
	AsciiLib.c canvas.c \
	about.c buttonTest.c floppyTest.c liveMode.c mainMenu.c player.c playlist.c \

HAL_SRCS = hal_display_stm32f4.c hal_filesystem_stm32f4.c hal_inputdevice_stm32f4.c hal_mididevice_stm32f4.c \
	hal_misc_stm32f4.c

PROJ_NAME=orgel
OUTPATH=bin

###################################################

MAIN_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(MAIN_SRCS))
LIB_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(LIB_SRCS))
DRIVERS_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(DRIVERS_SRCS))
COMMON_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(COMMON_SRCS))
HAL_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(HAL_SRCS))

###################################################

.PHONY: proj flash clean

all: $(OUTPATH)/$(PROJ_NAME).elf

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -L$(OBJDIR) -lstm32f4 -lm

$(OBJDIR)/libstm32f4.a: $(LIB_OBJS)
	$(AR) -r $@ $(LIB_OBJS)

$(OBJDIR)/libdrivers.a: $(DRIVERS_OBJS)
	$(AR) -r $@ $(DRIVERS_OBJS)

$(OBJDIR)/libcommon.a: $(COMMON_OBJS)
	$(AR) -r $@ $(COMMON_OBJS)

$(OBJDIR)/libhalfo.a: $(HAL_OBJS)
	$(AR) -r $@ $(HAL_OBJS)

$(OUTPATH)/$(PROJ_NAME).elf: $(OBJDIR)/libstm32f4.a $(OBJDIR)/libdrivers.a $(OBJDIR)/libcommon.a $(OBJDIR)/libhalfo.a $(MAIN_SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -L. -Lobj -lstm32f4 -ldrivers -lcommon -lhalfo -lm
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	$(SIZE) $@

submodules:
	git submodule init 
	git submodule update

flash: all
	st-flash write $(BINDIR)/$(PROJ_NAME).bin 0x8000000

proj: $(OUTPATH)/$(PROJ_NAME).elf

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	
