# Compiler
CXX := C:/mingw64/bin/g++.exe

# Directories
SRC_DIR := src
INC_DIR := include
LIB_DIR := lib

# Output
OUT := main.exe

# Flags
CXXFLAGS := -g -std=c++17 -I$(INC_DIR)
LDFLAGS := -L$(LIB_DIR) -lglfw3dll

# Source files
CPP_SRC := $(wildcard $(SRC_DIR)/*.cpp)
C_SRC   := $(wildcard $(SRC_DIR)/*.c)
SRC     := $(CPP_SRC) $(C_SRC)

# Default build
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(OUT)

# Run the executable
run: $(OUT)
	./$(OUT)

# Clean build files
clean:
	del /Q $(OUT)

.PHONY: all clean run
