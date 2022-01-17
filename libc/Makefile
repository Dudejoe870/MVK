TARGET_DIR = ../build

ifdef user_mode
TARGET = $(TARGET_DIR)/libc.a
else
TARGET = $(TARGET_DIR)/libk.a
USER_DEFINE = -D__is_libk
KERNEL_CFLAGS = -I../kernel/include
endif

DIRS = src src/sys src/liballoc

CFILES = $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.c))
ASFILES = $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.S))

COBJFILES = $(CFILES:%.c=%.o)
ASOBJFILES = $(ASFILES:%.S=%.o)

CFLAGS = -std=gnu17 -ffreestanding -O1 -Wall -Wextra -Iinclude -Iprivate_include -g

ASFLAGS = -ffreestanding -Iinclude -Iprivate_include

CC = i686-elf-gcc
AR = i686-elf-ar

all: $(TARGET) clean

$(TARGET): $(ASOBJFILES) $(COBJFILES)
	mkdir -p $(TARGET_DIR)
	$(AR) rcs $(TARGET) $(ASOBJFILES) $(COBJFILES)
clean:
	rm $(ASOBJFILES) $(COBJFILES)

$(ASOBJFILES): %.o : %.S
	$(CC) $(ASFLAGS) $(USER_DEFINE) -c $< -o $@

$(COBJFILES): %.o : %.c
	$(CC) $(CFLAGS) $(KERNEL_CFLAGS) $(USER_DEFINE) -c $< -o $@
