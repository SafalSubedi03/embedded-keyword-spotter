# Compiler and flags
CXX     = g++
CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c17
CXXFLAGS= -Wall -Wextra -g -std=c++17

# Libraries
LIBS    = -lportaudio -lfftw3 -lm -lpthread

# Directories
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Target executable
TARGET  = $(BIN_DIR)/embedded-keyword-spotter

# Source files (add new .c or .cpp files here as you create them)
SRCS    = $(SRC_DIR)/audioCapture.cpp

# Object files (auto generated from SRCS)
OBJS    = $(SRCS:.cpp=.o)

# Default target
all: $(BIN_DIR) $(TARGET)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Compile .cpp files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Compile .c files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Run
run: all
	./$(TARGET)

# Clean
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(TARGET)

# Phony targets
.PHONY: all run clean