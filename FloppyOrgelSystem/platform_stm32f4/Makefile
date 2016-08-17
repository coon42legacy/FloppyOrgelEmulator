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

vpath %.c cmsis cmsis/boot

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
CFLAGS += -Icmsis

OBJDIR=obj
CMSIS_SRCS = startup_stm32f4xx.c system_stm32f4xx.c
CMSIS_OBJS=$(patsubst %.c,$(OBJDIR)/%.o,$(CMSIS_SRCS))

all: $(OBJDIR)/libcmsis.a

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/libcmsis.a: $(CMSIS_OBJS)
	$(AR) -r $@ $(CMSIS_OBJS)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)

