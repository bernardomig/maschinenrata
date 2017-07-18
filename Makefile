
CC  := avr-gcc
OBJCOPY := avr-objcopy
UPLOADER := avrdude
UPLOADERTOOL := arduino
UPLOADERPORT ?= /dev/ttyACM0

TARGET := maschinenrata
SRCDIR := source
INCLUDEDIR := include
BINDIR := bin
DOCDIR := doc

MMCU := atmega328p

CFLAGS := -O2 -Wall

#########################################
### DO NOT CHANGE NOTHING AFTER THIS! ###
#########################################

# PREPEND THE BINDIR TO TARGET
TARGET:=$(BINDIR)/$(TARGET)

# DISCOVER ALL SOURCE FILES
SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%, $(BINDIR)/%, $(SOURCES:.c=.o))
# APPEND SOME COMPILER FLAGS
CFLAGS+=-I $(INCLUDEDIR) -mmcu=$(MMCU)

# UPLOADER FLAGS
UPLOADER_FLAGS= -p $(MMCU) -c $(UPLOADERTOOL) -P $(UPLOADERPORT)

.PHONY: all upload clean format doc

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex

$(BINDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET).elf

upload: $(TARGET).hex
	$(UPLOADER) $(UPLOADER_FLAGS) -U flash:w:$(TARGET).hex

clean:
	$(RM) $(BINDIR) -R

format:
	clang-format -style=Mozilla -i $(SRCDIR)/*.c
	clang-format -style=Mozilla -i $(INCLUDEDIR)/**/*.h

doc: 
	doxygen