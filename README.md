# Blockchain - Atelier 2 - Master IASD 2024/25

## Exercise 1: 1D Cellular Automaton Implementation

**Questions :**
- 1.1. Crée une fonction init_state() pour initialiser l’état à partir d’un vecteur de bits.
- 1.2. Implémente une fonction evolve() qui applique la règle de transition donnée (Rule 30, Rule 90, ou Rule 110).
- 1.3. Vérifie que ton automate reproduit correctement la règle choisie sur un petit état initial.

The goal is to implement a 1D cellular automaton with binary states (0 or 1) and neighborhood radius r=1 (each cell looks at itself and its two immediate neighbors).

**1.1 `init_state()` Function :**
- Initializes the CA grid from a vector of binary values
- Validates that all values are either 0 or 1
- Includes a helper method `init_single_center()` for testing with a single active cell in the middle

**1.2 `evolve()` Function :** 
- Applies the transition rule to generate the next generation
- Implements periodic boundary conditions (grid wraps around)
- Supports all elementary CA rules (0-255) including:
  - **Rule 30**: Chaotic, used for random number generation
  - **Rule 90**: Creates fractal patterns (Sierpiński triangle)
  - **Rule 110**: Turing-complete, complex behavior

**1.3 Rule Application Logic :**
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


**Compile & run the test:**
```bash
g++ -I./include tests/test_1.cpp src/cellular_automaton.cpp -o ./build/test_1.exe ; .\build\test_1.exe
```



<!-- 
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
3. Extracting a fixed 256-bit hash from the final state -->

## Exercise 2: Implementing a Hash Function Based on 1D Cellular Automaton


### Files Created
- `ac_hash.h` - Function declarations
- `ac_hash.cpp` - Implementation with conversion and extraction logic
- `test_2.cpp` & `test_2.exe`- Verification tests

### Compile & run

```bash
g++ -I./include tests/test_2.cpp src/cellular_automaton.cpp src/ac_hash.cpp -o ./build/test_2.exe ; ./build/test_2.exe
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

> make sure to run test_2.exe for results

**Test code:**
```cpp
std::string hash1 = ac_hash("Hello, World!", 30, 128);
std::string hash2 = ac_hash("Hello, World?", 30, 128);
std::string hash3 = ac_hash("Blockchain", 30, 128);
```

**Results:**
```terminal
============================================================
Test 2.4: Different Inputs Produce Different Hashes
============================================================
Input 1: "Hello, World!"
Hash 1:  f542fd8056e1376328537fec9a612f636565f9b6fc683ba5aadc16fefe3d0443

Input 2: "Hello, World?"
Hash 2:  057914ae9e6919e43c3c22a61bc564b255101e1dd360288101ae46cc360b7d9b

Input 3: "Blockchain"
Hash 3:  5b7f4d047a533a45de3cf321278fb890f4e8834df6a399b1d3030b17035dca31

Hash 1 != Hash 2: PASS
Hash 1 != Hash 3: PASS
Hash 2 != Hash 3: PASS

Hash length = 64 chars (256 bits): PASS

============================================================
Test: Same Input Produces Same Hash (Reproducibility)
============================================================
Input: "Test message"
Hash (run 1): 57cba362ad10f9f6496970d8cf4eec5c194b926de75e90b903da32d0e462a933
Hash (run 2): 57cba362ad10f9f6496970d8cf4eec5c194b926de75e90b903da32d0e462a933

Hashes match: PASS

============================================================
Test: Empty Input Handling
============================================================
Empty string hash: 0000000000000000000000000000000000000000000000000000000000000000
Space char hash:   744b0203e2611420b149050ae207b4a1e8119700c16dd03bae23e32f54fd7500

Different hashes: PASS
```


## Exercise 3 - Intègre la fonction ac_hash() dans la blockchain existante :

**Questions :**

- 3.1. Ajoute une option de sélection du mode de hachage (SHA256 ou AC_HASH).
- 3.2. Modifie le code du minage pour utiliser ac_hash() lors du calcul du hash de bloc.
- 3.3. Vérifie que la validation de bloc reste fonctionnelle avec cette nouvelle fonction.

**Compile & run:**

```bash

g++ -std=c++11 -I./include -IC:\msys64\mingw64\include src/cellular_automaton.cpp src/ac_hash.cpp src/utils.cpp src/pow.cpp src/block_pow.cpp src/blockchain_pow.cpp tests/test_3.cpp -LC:\msys64\mingw64\lib -lssl -lcrypto -o test_3.exe

```

**results :**

```terminal


==============================================================
=         EXERCISE 3: BLOCKCHAIN INTEGRATION TESTS           =
==============================================================

============================================================
TEST 3.1: Hash Mode Selection
============================================================

--- Creating blockchain with SHA256 ---
+ Blockchain created with hash mode: SHA-256

--- Creating blockchain with AC_HASH ---
+ Blockchain created with hash mode: AC HASH
+ CA Rule: 30
+ CA Steps: 128

[PASS] Hash mode selection working correctly

============================================================
TEST 3.2: Mining with ac_hash()
============================================================

--- Mining with SHA256 ---
Block #1 mined in 2 ms (SHA-256, 648 iterations)

--- Mining with AC_HASH (Rule 30) ---
Block #1 mined in 752 ms (AC HASH, 496 iterations)

[PASS] Mining works with both hash methods

============================================================
TEST 3.3: Block Validation with Both Hash Modes
============================================================

--- Testing SHA256 Blockchain Validation ---
Block #1 mined in 0 ms (SHA-256, 259 iterations)
Block #2 mined in 0 ms (SHA-256, 102 iterations)
SHA256 Chain Valid: YES

--- Testing AC_HASH Blockchain Validation ---
Block #1 mined in 328 ms (AC HASH, 219 iterations)
Block #2 mined in 200 ms (AC HASH, 120 iterations)
AC_HASH Chain Valid: YES

[PASS] Validation works correctly for both hash modes

============================================================
TEST 3.3 (Extended): Mixed Hash Modes in Same Chain
============================================================

--- Creating chain with SHA256, then switching to AC_HASH ---
Adding block 1 (SHA256)...
Block #1 mined in 1 ms (SHA-256, 282 iterations)

Switched to AC_HASH mode
Adding block 2 (AC_HASH)...
Block #2 mined in 1047 ms (AC HASH, 840 iterations)

Displaying mixed chain:

--- Block #0 (PoW - SHA-256) ---
Timestamp: Fri Oct 24 20:42:17.777
Data: Genesis Block
Previous Hash: 0...
Hash: 002f469fc7bfe747...
Nonce: 408
Difficulty: 2
Hash Mode: SHA-256

--- Block #1 (PoW - SHA-256) ---
Timestamp: Fri Oct 24 20:42:17.782
Data: Test transaction;
Previous Hash: 002f469fc7bfe747...
Hash: 0042b085d998e76e...
Nonce: 282
Difficulty: 2
Hash Mode: SHA-256

--- Block #2 (PoW - AC HASH) ---
Timestamp: Fri Oct 24 20:42:17.786
Data: Test transaction;
Previous Hash: 0042b085d998e76e...
Hash: 00fe93dc76700583...
Nonce: 840
Difficulty: 2
Hash Mode: AC HASH (Rule 30, 128 steps)

Mixed Chain Valid: Ô£ô YES

[PASS] Chain supports mixed hash modes

============================================================
TEST: Different CA Rules Comparison
============================================================

--- Mining with Rule 30 ---
Block #1 mined in 47 ms (AC HASH, 40 iterations)

--- Mining with Rule 90 ---

```


## Exercise 4 - Compare le comportement de ac_hash et de SHA256

**Questions :**
- 4.1. Mesure le temps moyen de minage de 10 blocs avec chaque méthode.
- 4.2. Mesure le nombre moyen d’itérations pour trouver un hash valide avec difficulté fixe.
- 4.3. Donne les résultats dans un tableau.


**compile & run:**

```bash
g++ -std=c++11 -I./include -IC:\msys64\mingw64\include src/cellular_automaton.cpp src/ac_hash.cpp src/utils.cpp src/pow.cpp src/block_pow.cpp src/blockchain_pow.cpp tests/test_4_benchmark.cpp -LC:\msys64\mingw64\lib -lssl -lcrypto -o test_4_benchmark.exe
```

**results**

```terminal

==============================================================
=            EXERCISE 4: PERFORMANCE BENCHMARKING            =
==============================================================

==============================================================
=            Mining Time and Iterations Comparison           =
==============================================================
Mining 10 blocks with difficulty 2...

--- Benchmarking SHA256 ---
Block #1 mined in 1 ms (SHA-256, 404 iterations)
Block #2 mined in 0 ms (SHA-256, 59 iterations)
Block #3 mined in 0 ms (SHA-256, 15 iterations)
Block #4 mined in 0 ms (SHA-256, 2 iterations)
Block #5 mined in 8 ms (SHA-256, 1121 iterations)
Block #6 mined in 0 ms (SHA-256, 39 iterations)
Block #7 mined in 0 ms (SHA-256, 34 iterations)
Block #8 mined in 0 ms (SHA-256, 67 iterations)
Block #9 mined in 0 ms (SHA-256, 120 iterations)
Block #10 mined in 2 ms (SHA-256, 462 iterations)

--- Benchmarking AC_HASH (Rule 30, 128 steps) ---
Block #1 mined in 956 ms (AC HASH, 523 iterations)
Block #2 mined in 262 ms (AC HASH, 147 iterations)
Block #3 mined in 550 ms (AC HASH, 318 iterations)
Block #4 mined in 491 ms (AC HASH, 281 iterations)
Block #5 mined in 20 ms (AC HASH, 8 iterations)
Block #6 mined in 294 ms (AC HASH, 166 iterations)
Block #7 mined in 335 ms (AC HASH, 198 iterations)
Block #8 mined in 459 ms (AC HASH, 265 iterations)
Block #9 mined in 483 ms (AC HASH, 270 iterations)
Block #10 mined in 180 ms (AC HASH, 100 iterations)

==========================================================================================
Hash Method    Difficulty  Total Time(ms) Avg Time(ms)   Total Iterations  Avg Iterations
------------------------------------------------------------------------------------------
SHA-256        2           30             3.00           2323              232.30
AC HASH        2           4039           403.90         2276              227.60
==========================================================================================

--- Analysis ---
AC_HASH is 134.63x SLOWER than SHA256
AC_HASH requires 0.98x FEWER iterations on average


=========================================================
=                   Complete Comparison Table               =
=============================================================

=== Testing with Difficulty 2 ===
Mining with SHA256...
Block #1 mined in 1 ms (SHA-256, 404 iterations)
Block #2 mined in 0 ms (SHA-256, 59 iterations)
Block #3 mined in 0 ms (SHA-256, 15 iterations)
Block #4 mined in 0 ms (SHA-256, 2 iterations)
Block #5 mined in 4 ms (SHA-256, 1121 iterations)
Block #6 mined in 0 ms (SHA-256, 39 iterations)
Block #7 mined in 0 ms (SHA-256, 34 iterations)
Block #8 mined in 0 ms (SHA-256, 67 iterations)
Block #9 mined in 1 ms (SHA-256, 120 iterations)
Block #10 mined in 2 ms (SHA-256, 462 iterations)
Mining with AC_HASH...
Block #1 mined in 884 ms (AC HASH, 523 iterations)
Block #2 mined in 252 ms (AC HASH, 147 iterations)
Block #3 mined in 571 ms (AC HASH, 318 iterations)
Block #4 mined in 464 ms (AC HASH, 281 iterations)
Block #5 mined in 15 ms (AC HASH, 8 iterations)
Block #6 mined in 278 ms (AC HASH, 166 iterations)
Block #7 mined in 321 ms (AC HASH, 198 iterations)
Block #8 mined in 470 ms (AC HASH, 265 iterations)
Block #9 mined in 474 ms (AC HASH, 270 iterations)
Block #10 mined in 178 ms (AC HASH, 100 iterations)

=== Testing with Difficulty 3 ===
Mining with SHA256...
Block #1 mined in 2 ms (SHA-256, 708 iterations)
Block #2 mined in 7 ms (SHA-256, 1424 iterations)
Block #3 mined in 20 ms (SHA-256, 5912 iterations)
Block #4 mined in 21 ms (SHA-256, 5371 iterations)
Block #5 mined in 8 ms (SHA-256, 2317 iterations)
Block #6 mined in 6 ms (SHA-256, 1741 iterations)
Block #7 mined in 35 ms (SHA-256, 11765 iterations)
Block #8 mined in 33 ms (SHA-256, 9720 iterations)
Block #9 mined in 3 ms (SHA-256, 653 iterations)
Block #10 mined in 8 ms (SHA-256, 1794 iterations)
Mining with AC_HASH...
Block #1 mined in 13473 ms (AC HASH, 7721 iterations)
Block #2 mined in 5590 ms (AC HASH, 3266 iterations)
Block #3 mined in 6370 ms (AC HASH, 3759 iterations)
Block #4 mined in 1934 ms (AC HASH, 1134 iterations)
Block #5 mined in 42951 ms (AC HASH, 25113 iterations)
Block #6 mined in 8233 ms (AC HASH, 4823 iterations)
Block #7 mined in 7746 ms (AC HASH, 4535 iterations)
Block #8 mined in 4253 ms (AC HASH, 2440 iterations)
Block #9 mined in 620 ms (AC HASH, 351 iterations)
Block #10 mined in 7163 ms (AC HASH, 4147 iterations)


====================================================
=              FINAL COMPARISON RESULTS            =
====================================================

==========================================================================================
Hash Method    Difficulty  Total Time(ms) Avg Time(ms)   Total Iterations  Avg Iterations
------------------------------------------------------------------------------------------
SHA-256        2           10             1.00           2323              232.30
AC HASH        2           3915           391.50         2276              227.60
SHA-256        3           150            15.00          41405             4140.50
AC HASH        3           98339          9833.90        57289             5728.90
==========================================================================================

--- Summary ---
SHA256: Fast per hash, but requires many iterations to find valid hash
AC_HASH: Slower per hash due to CA evolution, but similar iteration count
For blockchain: SHA256 is generally faster due to hardware optimization
AC_HASH provides alternative security model based on CA complexity


==========================================================
=         Additional Test: Different Difficulty Levels       =
==============================================================

==========================================================================================
Hash Method    Difficulty  Total Time(ms) Avg Time(ms)   Total Iterations  Avg Iterations
------------------------------------------------------------------------------------------

Testing difficulty 1...
Block #1 mined in 0 ms (SHA-256, 5 iterations)
Block #2 mined in 0 ms (SHA-256, 16 iterations)
Block #3 mined in 0 ms (SHA-256, 65 iterations)
Block #4 mined in 0 ms (SHA-256, 28 iterations)
Block #5 mined in 0 ms (SHA-256, 13 iterations)
SHA-256        1           40             8.00           127               25.40
Block #1 mined in 4 ms (AC HASH, 2 iterations)
Block #2 mined in 37 ms (AC HASH, 12 iterations)
Block #3 mined in 50 ms (AC HASH, 29 iterations)
Block #4 mined in 10 ms (AC HASH, 4 iterations)
Block #5 mined in 17 ms (AC HASH, 9 iterations)
AC HASH        1           125            25.00          56                11.20
------------------------------------------------------------------------------------------

Testing difficulty 2...
Block #1 mined in 1 ms (SHA-256, 404 iterations)
Block #2 mined in 0 ms (SHA-256, 59 iterations)
Block #3 mined in 0 ms (SHA-256, 15 iterations)
Block #4 mined in 0 ms (SHA-256, 2 iterations)
Block #5 mined in 5 ms (SHA-256, 1121 iterations)
SHA-256        2           9              1.80           1601              320.20
Block #1 mined in 935 ms (AC HASH, 523 iterations)
Block #2 mined in 257 ms (AC HASH, 147 iterations)
Block #3 mined in 619 ms (AC HASH, 318 iterations)
Block #4 mined in 567 ms (AC HASH, 281 iterations)
Block #5 mined in 15 ms (AC HASH, 8 iterations)
AC HASH        2           2397           479.40         1277              255.40
------------------------------------------------------------------------------------------

Testing difficulty 3...
Block #1 mined in 2 ms (SHA-256, 708 iterations)
Block #2 mined in 7 ms (SHA-256, 1424 iterations)
Block #3 mined in 28 ms (SHA-256, 5912 iterations)
Block #4 mined in 16 ms (SHA-256, 5371 iterations)
Block #5 mined in 7 ms (SHA-256, 2317 iterations)
SHA-256        3           67             13.40          15732             3146.40
Block #1 mined in 13368 ms (AC HASH, 7721 iterations)
Block #2 mined in 5619 ms (AC HASH, 3266 iterations)
Block #3 mined in 6573 ms (AC HASH, 3759 iterations)
Block #4 mined in 2005 ms (AC HASH, 1134 iterations)
Block #5 mined in 43106 ms (AC HASH, 25113 iterations)
AC HASH        3           70672          14134.40       40993             8198.60
------------------------------------------------------------------------------------------

Testing difficulty 4...
Block #1 mined in 135 ms (SHA-256, 43110 iterations)
Block #2 mined in 177 ms (SHA-256, 57356 iterations)
Block #3 mined in 1 ms (SHA-256, 666 iterations)
Block #4 mined in 31 ms (SHA-256, 6517 iterations)
Block #5 mined in 251 ms (SHA-256, 75928 iterations)
SHA-256        4           600            120.00         183577            36715.40
.
.
.

```

## Exercise 5 - Analyse l’effet avalanche de ac_hash

**Questions :**
- 5.1. Calcule le pourcentage moyen de bits différents entre les hashes de deux messages ne différant que
par un bit.
- 5.2. Donne le résultat numérique et ton code de test.

**compile & run :**

```bash

g++ -std=c++11 -I./include src/cellular_automaton.cpp src/ac_hash.cpp tests/test_5.cpp -o ./build/test_5.exe ; .\build\test_5.exe

```

**results :**

```terminal
==========================================================
=        EXERCISE 5: AVALANCHE EFFECT ANALYSIS             =
============================================================

Testing with 100 different messages...

--- Results ---
Average bits changed: 122.53 / 256
Percentage: 47.86%

--- Analysis ---
GOOD: Close to ideal 50% (strong avalanche effect)

```


## Exercise 6 - Analyse la distribution des bits produits par ac_hash

**Questions :**
- 6.1. Calcule le pourcentage de bits à 1 sur un échantillon d’au moins 105 bits.
- 6.2. Indique si la distribution est équilibrée (≈50 % de 1).


**Compile & run:**

```bash

g++ -std=c++11 -I./include src/cellular_automaton.cpp src/ac_hash.cpp tests/test_6.cpp -o ./build/test_6.exe ; .\build\test_6.exe

```

**Results:**

```terminal

Hash: f4149e7fdc33465c484eb6dc62bfa3d94ac855413a3befea3e2bc9ae50feb6b7
Percentage of 1 bits: 3.12%

```

## Exercise 7 - Test and evaluate AC Haching algorithm on 30, 90 & the 110 rule 

**Questions :**
- 7.1. Execute ac_hash avec Rule 30, Rule 90 et Rule 110.
- 7.2. Compare la stabilite des résultats et les temps d'execution.7.3. Indique quelle regle te semble la plus adaptee pour le hachage et pourquoi.
- 7.3. Indique quelle règle te semble la plus adaptée pour le hachage et pourquoi.

### Compile and run:

```bash
g++ -std=c++11 -I./include src/cellular_automaton.cpp src/ac_hash.cpp tests/test_7.cpp -o ./build/test_7.exe ; .\build\test_7.exe
```


by running test_7.exe, we get the following output:

```terminal

==============================================================
=               TEST 7: COMPARISON OF CA RULES               =
==============================================================

Test in progress with: "Transaction 1: sending 5.00 BTC to 0x1234567890abcde"
Number of steps: 128

Executing with Rule 30...
Hash: 330d5388ab52949b...

Executing with Rule 90...
Hash: 01edd4193c37a291...

Executing with Rule 110...
Hash: 9e712d7ff767f6b1...

================================================================================
Comparative results of rules
================================================================================

Rule      Time(us)            Avalanche (%)            Stability (%)            
--------------------------------------------------------------------------------
30        1559                45.70                    50.39
90        2075                22.27                    50.00
110       2063                50.39                    55.08

================================================================================
7.3: Analysis & Recommendation
================================================================================

--- Rule 30 ---
Characteristics: Chaotic behavior
Avalanche: EXCELLENT
Balance: EXCELLENT

--- Rule 90 ---
Characteristics: Generates fractals (Sierpi┼äski triangle)
Avalanche: LOW
Balance: EXCELLENT

--- Rule 110 ---
Characteristics: Turing-complete, complex behavior
Avalanche: EXCELLENT
Balance: ACCEPTABLE

--------------------------------------------------------------------------------
FINAL RECOMMENDATION:
--------------------------------------------------------------------------------

Rule 30 is the MOST SUITABLE for hashing because:
  1. Optimal chaotic behavior for diffusion
  2. Ideal avalanching effect (close to 50%)
  3. Good distribution of bits
  4. Widely used in cryptography (random number generator)

Rule 90:
  - Too predictable structure (fractal)
  - Good for cryptographic diffusion

Rule 110:
  - Interesting complexity but may be too structured
  - Limited use for cryptographic applications

================================================================================
TEST COMPLETE
================================================================================

```