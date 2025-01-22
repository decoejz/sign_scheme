# Compiler settings
CC = gcc
CFLAGS = -g -std=c11 -Wall

# Directories and includes
INSTALL_DIR = ../install
CFLAGS += -I$(INSTALL_DIR)/include -I/opt/homebrew/opt/openssl@3/include ## TODO colocar apenas se for macos
LDFLAGS = -L$(INSTALL_DIR)/lib -L/opt/homebrew/opt/openssl@3/lib ## TODO colocar apenas se for macos

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