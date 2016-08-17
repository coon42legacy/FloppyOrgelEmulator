BINPATH=/usr/bin

CC=$(BINPATH)/arm-none-eabi-gcc
AR=$(BINPATH)/arm-none-eabi-ar

###################################################

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

vpath %.c cmsis cmsis/boot lib/misc lib/random lib/std_periph/source lib/stdio lib/syscalls

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4

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

OBJDIR=obj
CMSIS_SRCS = startup_stm32f4xx.c system_stm32f4xx.c
STDPERIPH_SRCS = delay.c stm32_ub_rng.c \
		misc.c stm32f4xx_exti.c stm32f4xx_gpio.c stm32f4xx_rng.c \
		stm32f4xx_spi.c stm32f4xx_tim.c stm32f4xx_dma.c \
		stm32f4xx_fsmc.c stm32f4xx_rcc.c stm32f4xx_sdio.c \
		stm32f4xx_syscfg.c stm32f4xx_usart.c

CMSIS_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(CMSIS_SRCS))
STDPERIPH_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(STDPERIPH_SRCS))

all: $(OBJDIR)/libcmsis.a $(OBJDIR)/stdperiph.a

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/libcmsis.a: $(CMSIS_OBJS)
	$(AR) -r $@ $(CMSIS_OBJS)

$(OBJDIR)/stdperiph.a: $(STDPERIPH_OBJS)
	$(AR) -r $@ $(STDPERIPH_OBJS)


.PHONY: clean
clean:
	rm -rf $(OBJDIR)

