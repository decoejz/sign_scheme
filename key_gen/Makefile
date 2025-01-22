# Compiler settings
CC = gcc
CFLAGS = -g -std=c11 -Wall

# Directories and includes
INSTALL_DIR = ../install
CFLAGS += -I$(INSTALL_DIR)/include
LDFLAGS = -L$(INSTALL_DIR)/lib

ifeq ($(shell uname),Darwin)
    CFLAGS += -I/opt/homebrew/opt/openssl@3/include
    LDFLAGS += -L/opt/homebrew/opt/openssl@3/lib
endif

# Libraries - order matters!
LIBS = -lSignScheme -lssl -lcrypto

# Source files
SRCS = main.c
TARGET = main

# Default target
all: $(TARGET)

# Compilation and linking
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

# Clean target
clean:
	rm -f $(TARGET)

.PHONY: all clean