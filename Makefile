# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g

# Source files
SRC = test.cpp main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Output executable
TARGET = riscv_sim

# Default target to build the executable
all: $(TARGET)

# Rule to build the executable from the object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean the build
clean:
	rm -rf $(TARGET) $(OBJ)

# Phony targets to avoid name conflicts with files
.PHONY: all clean
