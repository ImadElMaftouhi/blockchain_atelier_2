# blockchain_atelier_2

## Exercise 1: 1D Cellular Automaton Implementation

The goal is to implement a 1D cellular automaton with binary states (0 or 1) and neighborhood radius r=1 (each cell looks at itself and its two immediate neighbors).

### What We Built

#### 1.1 `init_state()` Function
- Initializes the CA grid from a vector of binary values
- Validates that all values are either 0 or 1
- Includes a helper method `init_single_center()` for testing with a single active cell in the middle

#### 1.2 `evolve()` Function
- Applies the transition rule to generate the next generation
- Implements periodic boundary conditions (grid wraps around)
- Supports all elementary CA rules (0-255) including:
  - **Rule 30**: Chaotic, used for random number generation
  - **Rule 90**: Creates fractal patterns (Sierpiński triangle)
  - **Rule 110**: Turing-complete, complex behavior

#### 1.3 Rule Application Logic
The rule number encodes how each 3-cell pattern evolves:

```
Pattern:  111  110  101  100  011  010  001  000
Rule 30:   0    0    0    1    1    1    1    0
Binary:   00011110 = 30 in decimal
```

For each cell, we:
1. Read the 3-cell neighborhood (left, center, right)
2. Convert to pattern index: `(left << 2) | (center << 1) | right`
3. Look up the output bit: `(rule >> pattern) & 1`

### Files Created
- `cellular_automaton.h` - Class declaration
- `cellular_automaton.cpp` - Implementation
- `test_ca.cpp` - Verification test

### How to Verify (Question 1.3)

#### Compile the test:
```bash
g++ -I./include tests/test_ca.cpp src/cellular_automaton.cpp -o ./build/test_ca.exe ; .\build\test_ca.exe
```

#### Run the test:
```bash
./tests/test_ca.exe
```

#### Expected Output for Rule 30:
```
Generation 0:     #    
Generation 1:    ###
Generation 2:   ##  #
Generation 3:  ## ####
```

This matches the known behavior of Rule 30 when starting with a single active cell.

#### Verify Different Rules:
```cpp
// Test Rule 90 (creates Sierpiński triangle)
CellularAutomaton ca(17, 90);
ca.init_single_center();
for (int i = 0; i < 8; i++) {
    ca.print_state();
    ca.evolve();
}
```

### Key Features Implemented
✅ Binary state cellular automaton (0 and 1)  
✅ Neighborhood radius r=1 (3-cell window)  
✅ Periodic boundary conditions  
✅ Support for any elementary rule (0-255)  
✅ State initialization from vector  
✅ Evolution function with configurable steps  
✅ State extraction and visualization methods  

### Next Steps
This CA implementation will be used in Exercise 2 to create a hash function by:
1. Converting input text to binary state
2. Evolving the CA for multiple generations
3. Extracting a fixed 256-bit hash from the final state

## Exercise 2: Implementing a Hash Function Based on 1D Cellular Automaton


### Files Created
- `ac_hash.h` - Function declarations
- `ac_hash.cpp` - Implementation with conversion and extraction logic
- `test_ac_hash.cpp` - Verification tests

### Compilation and Execution
```bash
# Compile
g++ -std=c++11 cellular_automaton.cpp ac_hash.cpp test_ac_hash.cpp -o test_ac_hash

# Run tests
./test_ac_hash
```

### 2.1 Function Implementation
We implemented the requested function with the following signature:
```cpp
std::string ac_hash(const std::string& input, uint32_t rule, size_t steps);
```

**Parameters:**
- `input`: The text string to hash
- `rule`: The CA rule number (e.g., 30, 90, 110)
- `steps`: Number of evolution generations to run

**Return value:** A 256-bit hash as a 64-character hexadecimal string

### 2.2 Text to Bits Conversion Mode
Each character in the input string is converted to its ASCII value. Then, each ASCII value is represented as 8 bits (MSB first). After that, all bits are concatenated into a single bit vector

**Example:**
```
Input: "AB"
- 'A' = ASCII 65 = 01000001
- 'B' = ASCII 66 = 01000010
- Result: [0,1,0,0,0,0,0,1,0,1,0,0,0,0,1,0]
```

**Code implementation:**
```cpp
std::vector<int> string_to_bits(const std::string& input) {
    std::vector<int> bits;
    for (unsigned char c : input) {
        for (int i = 7; i >= 0; i--) {
            bits.push_back((c >> i) & 1);
        }
    }
    return bits;
}
```

### 2.3 Hash Production Process (256-bit Fixed Output)

**Step-by-step process:**

1. **Convert input to bits** using the method described in 2.2
2. **Pad to minimum size:** If input < 256 bits, pad with alternating 0/1 pattern to reach at least 256 bits
3. **Initialize CA:** Create a cellular automaton with the bit vector as initial state
4. **Evolve CA:** Run the automaton for the specified number of steps, storing intermediate states in history
5. **Extract 256 bits** using XOR folding technique:
   - Take final state as base
   - If state > 256 bits: fold excess bits using XOR (`hash[i % 256] ^= state[i]`)
   - If state < 256 bits: repeat pattern with XOR
   - Mix with sampled history states using rotated XOR to improve diffusion
6. **Convert to hex:** Transform 256 bits into 64 hexadecimal characters

**Notes:**
- XOR folding preserves bit distribution and creates avalanche effect
- History mixing ensures all evolution steps contribute to final hash
- Fixed 256-bit output regardless of input size
- Deterministic and reproducible

### 2.4 Verification Test

**Test code:**
```cpp
std::string hash1 = ac_hash("Hello, World!", 30, 128);
std::string hash2 = ac_hash("Hello, World?", 30, 128);
std::string hash3 = ac_hash("Blockchain", 30, 128);
```

**Results:**
```
Input 1: "Hello, World!"
Hash 1:  [64 hex characters - unique]

Input 2: "Hello, World?" (only last character different)
Hash 2:  [64 hex characters - completely different from Hash 1]

Input 3: "Blockchain"
Hash 3:  [64 hex characters - different from both above]
```

**Verification:**
✓ Hash 1 ≠ Hash 2: PASS  
✓ Hash 1 ≠ Hash 3: PASS  
✓ Hash 2 ≠ Hash 3: PASS  
✓ All hashes are exactly 64 hex characters (256 bits): PASS
