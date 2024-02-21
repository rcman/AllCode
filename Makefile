# Makefile for SDL2 project with SDL2_mixer, SDL2_image, and SDL2_ttf

# Compiler and flags
CC := g++
CFLAGS := -std=c++11 -Wall -Iinclude $(shell sdl2-config --cflags) -I/usr/include/SDL2
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_mixer -lSDL2_image -lSDL2_ttf

# Directories
SRC_DIR := src
INCLUDE_DIR := include
RELEASE_DIR := release

# Source and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(RELEASE_DIR)/%.o,$(SRC_FILES))

# Executable name
TARGET := $(RELEASE_DIR)/my_sdl2_game

# Phony targets
.PHONY: all clean

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $^ -o $@ $(LDFLAGS)

# Rule to build object files
$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(RELEASE_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	

