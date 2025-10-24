# Makefile for Blockchain Project with Cellular Automate Integration
# Available commands:
# make all         # Build all tests (doesn't run them)
# make test        # Build and run ALL tests sequentially
# make test_1      # Build and run only Test 1
# make test_2      # Build and run only Test 2
# ...
# make test_7      # Build and run only Test 7
# make clean       # Remove all build artifacts
# make rebuild     # Clean + build everything
# make help        # Show help message


CXX = g++
CXXFLAGS = -std=c++11 -Wall -I./include

# Detect OS and set library paths
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2/MinGW)
    CXXFLAGS += -IC:/msys64/mingw64/include
    LIBS = -LC:/msys64/mingw64/lib -lssl -lcrypto
    RM = del /Q
    MKDIR = if not exist build mkdir build
    RMDIR = rmdir /S /Q
    EXE_EXT = .exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux
        LIBS = -lssl -lcrypto
    endif
    ifeq ($(UNAME_S),Darwin)
        # macOS
        LIBS = -lssl -lcrypto
    endif
    RM = rm -f
    MKDIR = mkdir -p build
    RMDIR = rm -rf
    EXE_EXT =
endif

# Directories
BUILD_DIR = ./build
SRC_DIR = ./src
TEST_DIR = ./tests

# Source files
CA_SRC = $(SRC_DIR)/cellular_automaton.cpp
AC_HASH_SRC = $(SRC_DIR)/ac_hash.cpp
UTILS_SRC = $(SRC_DIR)/utils.cpp
POW_SRC = $(SRC_DIR)/pow.cpp
BLOCK_POW_SRC = $(SRC_DIR)/block_pow.cpp
BLOCKCHAIN_POW_SRC = $(SRC_DIR)/blockchain_pow.cpp

# Common source combinations
BASIC_SRCS = $(CA_SRC)
HASH_SRCS = $(CA_SRC) $(AC_HASH_SRC)
BLOCKCHAIN_SRCS = $(CA_SRC) $(AC_HASH_SRC) $(UTILS_SRC) $(POW_SRC) $(BLOCK_POW_SRC) $(BLOCKCHAIN_POW_SRC)

# Test executables
TEST_1 = $(BUILD_DIR)/test_1$(EXE_EXT)
TEST_2 = $(BUILD_DIR)/test_2$(EXE_EXT)
TEST_3 = $(BUILD_DIR)/test_3$(EXE_EXT)
TEST_4 = $(BUILD_DIR)/test_4_benchmark$(EXE_EXT)
TEST_5 = $(BUILD_DIR)/test_5$(EXE_EXT)
TEST_6 = $(BUILD_DIR)/test_6$(EXE_EXT)
TEST_7 = $(BUILD_DIR)/test_7$(EXE_EXT)

ALL_TESTS = $(TEST_1) $(TEST_2) $(TEST_3) $(TEST_4) $(TEST_5) $(TEST_6) $(TEST_7)

# Default target
.PHONY: all
all: $(ALL_TESTS)

# Build directory creation
$(BUILD_DIR):
	@$(MKDIR)

# Test 1: Cellular Automaton
$(TEST_1): $(TEST_DIR)/test_1.cpp $(BASIC_SRCS) | $(BUILD_DIR)
	@echo "Building Test 1: Cellular Automaton..."
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_1.cpp $(BASIC_SRCS) -o $@

# Test 2: AC Hash Function
$(TEST_2): $(TEST_DIR)/test_2.cpp $(HASH_SRCS) | $(BUILD_DIR)
	@echo "Building Test 2: AC Hash Function..."
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_2.cpp $(HASH_SRCS) -o $@

# Test 3: Blockchain Integration
$(TEST_3): $(TEST_DIR)/test_3.cpp $(BLOCKCHAIN_SRCS) | $(BUILD_DIR)
	@echo "Building Test 3: Blockchain Integration..."
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_3.cpp $(BLOCKCHAIN_SRCS) $(LIBS) -o $@

# Test 4: Performance Benchmark
$(TEST_4): $(TEST_DIR)/test_4_benchmark.cpp $(BLOCKCHAIN_SRCS) | $(BUILD_DIR)
	@echo "Building Test 4: Performance Benchmark..."
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_4_benchmark.cpp $(BLOCKCHAIN_SRCS) $(LIBS) -o $@

# Test 5: Avalanche Effect
$(TEST_5): $(TEST_DIR)/test_5.cpp $(HASH_SRCS) | $(BUILD_DIR)
	@echo "Building Test 5: Avalanche Effect..."
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_5.cpp $(HASH_SRCS) -o $@

# Test 6: Bit Distribution
$(TEST_6): $(TEST_DIR)/test_6.cpp $(HASH_SRCS) | $(BUILD_DIR)
	@echo "Building Test 6: Bit Distribution..."
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_6.cpp $(HASH_SRCS) -o $@

# Test 7: Rule Comparison
$(TEST_7): $(TEST_DIR)/test_7.cpp $(HASH_SRCS) | $(BUILD_DIR)
	@echo "Building Test 7: Rule Comparison..."
	$(CXX) $(CXXFLAGS) $(TEST_DIR)/test_7.cpp $(HASH_SRCS) -o $@

# Individual test targets
.PHONY: test_1 test_2 test_3 test_4 test_5 test_6 test_7
test_1: $(TEST_1)
	@echo "\n=== Running Test 1 ==="
	@$(TEST_1)

test_2: $(TEST_2)
	@echo "\n=== Running Test 2 ==="
	@$(TEST_2)

test_3: $(TEST_3)
	@echo "\n=== Running Test 3 ==="
	@$(TEST_3)

test_4: $(TEST_4)
	@echo "\n=== Running Test 4 ==="
	@$(TEST_4)

test_5: $(TEST_5)
	@echo "\n=== Running Test 5 ==="
	@$(TEST_5)

test_6: $(TEST_6)
	@echo "\n=== Running Test 6 ==="
	@$(TEST_6)

test_7: $(TEST_7)
	@echo "\n=== Running Test 7 ==="
	@$(TEST_7)

# Run all tests
.PHONY: test
test: $(ALL_TESTS)
	@echo "\n================================================================"
	@echo "=          BLOCKCHAIN CA - RUNNING ALL TESTS                   ="
	@echo "================================================================\n"
	@echo ">>> Test 1: Cellular Automaton Implementation"
	@$(TEST_1)
	@echo "\n>>> Test 2: AC Hash Function"
	@$(TEST_2)
	@echo "\n>>> Test 3: Blockchain Integration"
	@$(TEST_3)
	@echo "\n>>> Test 4: Performance Benchmarking"
	@$(TEST_4)
	@echo "\n>>> Test 5: Avalanche Effect Analysis"
	@$(TEST_5)
	@echo "\n>>> Test 6: Bit Distribution Analysis"
	@$(TEST_6)
	@echo "\n>>> Test 7: CA Rules Comparison"
	@$(TEST_7)
	@echo "\n================================================================"
	@echo "=                    ALL TESTS COMPLETED                       ="
	@echo "================================================================\n"

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build directory..."
ifeq ($(OS),Windows_NT)
	@if exist build $(RMDIR) build
else
	@$(RMDIR) $(BUILD_DIR)
endif
	@echo "Clean complete."

# Rebuild everything
.PHONY: rebuild
rebuild: clean all

# Help target
.PHONY: help
help:
	@echo "Blockchain CA Project - Makefile Help"
	@echo ""
	@echo "Available targets:"
	@echo "  make all         - Build all tests"
	@echo "  make test        - Build and run all tests"
	@echo "  make test_N      - Build and run specific test (N = 1-7)"
	@echo "  make clean       - Remove build artifacts"
	@echo "  make rebuild     - Clean and rebuild everything"
	@echo "  make help        - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make test_1      - Run only Test 1 (Cellular Automaton)"
	@echo "  make test_5      - Run only Test 5 (Avalanche Effect)"
	@echo ""