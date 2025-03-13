# Compiler settings
CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic

# Libraries
LIBS=-lm -lcunit
LFLAGS=-L$(shell brew --prefix cunit)/lib
# Find source and header files
SRCS_C := $(shell find src -name "*.c")
SRCS_H := $(shell find src -name "*.h")

# Generate object files in obj/ preserving directory structure
OBJS := $(patsubst src/%.c,obj/%.o,$(SRCS_C))

# Output binary
BIN := bin/$(shell basename $(shell pwd))

# Include paths
IDIRS := -Isrc -I$(shell brew --prefix cunit)/include

# Ensure directories exist
$(shell mkdir -p bin)
$(shell find src -type d | sed 's/src/obj/' | xargs mkdir -p)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ $(IDIRS) $(LIBS)

# Compile C files to object files, preserving directory structure
obj/%.o: src/%.c $(SRCS_H)
	$(CC) $(CFLAGS) -c -o $@ $< $(IDIRS)

clean:
	rm -rf obj bin
	rm -rf *.log
