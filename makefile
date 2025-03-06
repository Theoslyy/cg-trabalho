# Compiler
CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -march=native `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Object and binary directories
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS := $(shell find . -name "*.cpp")
# Object files
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
# Output executable
TARGET = main

# Default target
all: $(BIN_DIR)/$(TARGET)

# Compile and run project
run: all | $(BIN_DIR)/$(TARGET)
	./$(BIN_DIR)/$(TARGET)

# Create object and binary directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Build the target
$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Delete build files
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

# Phony targets
.PHONY: all clean run
