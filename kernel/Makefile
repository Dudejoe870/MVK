TARGET_DIR = ../build
TARGET = $(TARGET_DIR)/MVK.elf

DIRS = src

CFILES = $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.c))
ASFILES = $(foreach DIR,$(DIRS),$(wildcard $(DIR)/*.S))

COBJFILES = $(CFILES:%.c=%.o)
ASOBJFILES = $(ASFILES:%.S=%.o)

OPT = -O1
CFLAGS = -std=gnu17 -ffreestanding $(OPT) -Wall -Wextra -Iinclude -Iprivate_include -I../libc/include -nostdlib -g
ASFLAGS = -ffreestanding -Iinclude -Iprivate_include -Wa,--divide
LDFLAGS = -ffreestanding -nostdlib

CC = i686-elf-gcc

all: $(TARGET) clean

$(TARGET): $(ASOBJFILES) $(COBJFILES)
	mkdir -p $(TARGET_DIR)
	$(CC) -v -m32 -T linker.ld -o $(TARGET) $(LDFLAGS) $(ASOBJFILES) $(COBJFILES) -L../build -lk -lgcc
clean:
	rm $(ASOBJFILES) $(COBJFILES)

$(ASOBJFILES): %.o : %.S
	$(CC) $(ASFLAGS) -c $< -o $@

$(COBJFILES): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
