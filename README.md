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