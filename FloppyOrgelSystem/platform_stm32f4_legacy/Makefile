BINPATH=/usr/bin

CC=$(BINPATH)/arm-none-eabi-gcc
AR=$(BINPATH)/arm-none-eabi-ar
LD=$(BINPATH)/arm-none-eabi-ld
SIZE=$(BINPATH)/arm-none-eabi-size
OBJCOPY=$(BINPATH)/arm-none-eabi-objcopy

###################################################

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

vpath %.c cmsis cmsis/boot lib/misc lib/random \
          lib/std_periph/source lib/stdio lib/syscalls \
          drivers/LCD drivers/NesGamePad drivers/Filesystem drivers/Filesystem/fatfs \
          drivers/Filesystem/fatfs/drivers drivers/Filesystem/fatfs/option \
          ../common ../common/AsciiLib ../common/canvas ../common/states ../common/states/playback

CFLAGS  = -std=gnu99 -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
#CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
else
CFLAGS += -msoft-float
endif

CFLAGS += -ffreestanding -nostdlib
CFLAGS += -Icmsis -Icmsis/boot 
CFLAGS += -Ilib/misc
CFLAGS += -Ilib/std_periph/include
CFLAGS += -Idrivers/LCD -Idrivers/NesGamePad
CFLAGS += -Idrivers/Filesystem -Idrivers/Filesystem/fatfs -Idrivers/Filesystem/fatfs/drivers
CFLAGS += -I../common -I../common/AsciiLib -I../common/canvas -I../common/states -I../common/states/playback 
CFLAGS += -DUSE_STDPERIPH_DRIVER

OBJDIR=obj
BINDIR=bin
CMSIS_SRCS = startup_stm32f4xx.c system_stm32f4xx.c
STDPERIPH_SRCS = delay.c stm32_ub_rng.c \
		misc.c stm32f4xx_exti.c stm32f4xx_gpio.c stm32f4xx_rng.c \
		stm32f4xx_spi.c stm32f4xx_tim.c stm32f4xx_dma.c \
		stm32f4xx_fsmc.c stm32f4xx_rcc.c stm32f4xx_sdio.c \
		stm32f4xx_syscfg.c stm32f4xx_usart.c
DRIVERS_SRCS = SSD1289.c NesGamePad.c \
               tm_stm32f4_delay.c tm_stm32f4_fatfs.c tm_stm32f4_gpio.c tm_stm32f4_timer_properties.c \
               diskio.c ff.c fatfs_sd_sdio.c ccsbcs.c syscall.c

COMMON_SRCS = main.c common_main.c InputDevice.c LockFreeFifo.c SlotBasedMenu.c StackBasedFsm.c \
              AsciiLib.c canvas.c \
              about.c buttonTest.c floppyTest.c liveMode.c mainMenu.c player.c playlist.c \
              
CMSIS_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(CMSIS_SRCS))
STDPERIPH_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(STDPERIPH_SRCS))
DRIVERS_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(DRIVERS_SRCS))
COMMON_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(COMMON_SRCS))

all: $(BINDIR)/image.elf

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -Llib -lstm32f4 -lm

$(OBJDIR)/libcmsis.a: $(CMSIS_OBJS)
	$(AR) -r $@ $(CMSIS_OBJS)

$(OBJDIR)/libstdperiph.a: $(STDPERIPH_OBJS)
	$(AR) -r $@ $(STDPERIPH_OBJS)

$(OBJDIR)/libdrivers.a: $(DRIVERS_OBJS)
	$(AR) -r $@ $(DRIVERS_OBJS)

$(OBJDIR)/libcommon.a: $(COMMON_OBJS)
	$(AR) -r $@ $(COMMON_OBJS)

$(BINDIR)/image.elf: $(OBJDIR)/libcmsis.a $(OBJDIR)/libstdperiph.a \
                 $(OBJDIR)/libdrivers.a $(OBJDIR)/libcommon.a
	$(CC) $(CFLAGS) $^ -o $@ -Lobj -lcmsis -lstdperiph -ldrivers -lcommon -lm
	$(OBJCOPY) -O ihex $(BINDIR)/image.elf $(BINDIR)/image.hex
	$(OBJCOPY) -O binary $(BINDIR)/image.elf $(BINDIR)/image.bin

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

