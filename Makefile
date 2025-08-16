# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -g -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build
TARGET = battle-sim

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Clean target
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean
