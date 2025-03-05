# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -O3 -march=native `sdl2-config --cflags`

# Linker flags
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

run: $(BIN_DIR)/$(TARGET) | all
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

# Clean up build files
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

# Phony targets
.PHONY: all clean
