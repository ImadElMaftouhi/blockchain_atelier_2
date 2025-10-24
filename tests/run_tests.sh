#!/bin/bash

# run_tests.sh - Automated test execution script
# Compiles and runs all tests for the Blockchain CA project

# chmod +x run_tests.sh ; ./run_tests.sh




# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Directories
BUILD_DIR="./build"
SRC_DIR="./src"
TEST_DIR="./tests"
INCLUDE_DIR="./include"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Compiler settings
CXX="g++"
CXXFLAGS="-std=c++11 -I$INCLUDE_DIR"
LIBS=""

# Detect OS and set library paths
if [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "mingw"* ]]; then
    # Windows (MSYS2/MinGW)
    CXXFLAGS="$CXXFLAGS -IC:/msys64/mingw64/include"
    LIBS="-LC:/msys64/mingw64/lib -lssl -lcrypto"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    LIBS="-lssl -lcrypto"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    LIBS="-lssl -lcrypto"
fi

# Source files
CA_SRC="$SRC_DIR/cellular_automaton.cpp"
AC_HASH_SRC="$SRC_DIR/ac_hash.cpp"
UTILS_SRC="$SRC_DIR/utils.cpp"
POW_SRC="$SRC_DIR/pow.cpp"
BLOCK_POW_SRC="$SRC_DIR/block_pow.cpp"
BLOCKCHAIN_POW_SRC="$SRC_DIR/blockchain_pow.cpp"

echo -e "${BLUE}================================================================${NC}"
echo -e "${BLUE}=          BLOCKCHAIN CA - AUTOMATED TEST SUITE                =${NC}"
echo -e "${BLUE}================================================================${NC}"
echo ""

# Function to compile and run a test
run_test() {
    local test_num=$1
    local test_name=$2
    local sources=$3
    local needs_ssl=$4
    
    local test_file="$TEST_DIR/test_${test_num}.cpp"
    local output_file="$BUILD_DIR/test_${test_num}.exe"
    
    echo -e "${YELLOW}>>> Test $test_num: $test_name${NC}"
    echo -e "Compiling..."
    
    if [ "$needs_ssl" = true ]; then
        $CXX $CXXFLAGS $test_file $sources $LIBS -o "$output_file"
    else
        $CXX $CXXFLAGS $test_file $sources -o "$output_file"
    fi
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Compilation successful${NC}"
        echo -e "Running test...\n"
        "$output_file"
        echo ""
    else
        echo -e "${RED}✗ Compilation failed${NC}"
        return 1
    fi
}

# Test 1: Cellular Automaton Basic Test
run_test "1" "Cellular Automaton Implementation" \
    "$CA_SRC" \
    false

# Test 2: AC Hash Function
run_test "2" "AC Hash Function" \
    "$CA_SRC $AC_HASH_SRC" \
    false

# Test 3: Blockchain Integration
run_test "3" "Blockchain Integration (SHA256 vs AC_HASH)" \
    "$CA_SRC $AC_HASH_SRC $UTILS_SRC $POW_SRC $BLOCK_POW_SRC $BLOCKCHAIN_POW_SRC" \
    true

# Test 4: Performance Benchmark
run_test "4_benchmark" "Performance Benchmarking" \
    "$CA_SRC $AC_HASH_SRC $UTILS_SRC $POW_SRC $BLOCK_POW_SRC $BLOCKCHAIN_POW_SRC" \
    true

# Test 5: Avalanche Effect
run_test "5" "Avalanche Effect Analysis" \
    "$CA_SRC $AC_HASH_SRC" \
    false

# Test 6: Bit Distribution
run_test "6" "Bit Distribution Analysis" \
    "$CA_SRC $AC_HASH_SRC" \
    false

# Test 7: Rule Comparison
run_test "7" "CA Rules Comparison (30, 90, 110)" \
    "$CA_SRC $AC_HASH_SRC" \
    false

echo -e "${BLUE}================================================================${NC}"
echo -e "${GREEN}ALL TESTS COMPLETED${NC}"
echo -e "${BLUE}================================================================${NC}"
echo ""
echo "Test executables are available in: $BUILD_DIR/"
echo ""