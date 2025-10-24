# Blockchain with Cellular Automaton Hash Function

> **Master IASD 2024/25 - M234**  
> A blockchain implementation featuring a novel hash function based on 1D cellular automata, with comparative analysis against SHA-256.

[![C++](https://img.shields.io/badge/C++-11-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)


## Overview

This project explores an alternative approach to blockchain hashing by implementing a cryptographic hash function based on **1D cellular automata** (CA). The implementation includes:

- A configurable 1D cellular automaton engine supporting rules 0-255
- A 256-bit hash function (`ac_hash`) built on CA evolution
- A complete Proof-of-Work blockchain supporting both SHA-256 and AC_HASH
- Comprehensive performance and cryptographic quality analysis

### Motivation

While SHA-256 dominates blockchain technology, this project investigates whether cellular automata—known for their chaotic behavior and computational simplicity—can provide a viable alternative with unique properties like potential ASIC resistance.

---

## ✨ Features

### Cellular Automaton Engine
- Binary state (0/1) with configurable rules (0-255).
- Periodic boundary conditions.
- Support for Rule 30 (chaotic), Rule 90 (fractal), Rule 110 (Turing-complete).
- Efficient state evolution and history tracking.

### AC Hash Function
- Fixed 256-bit output (64 hex characters)
- Deterministic and reproducible
- XOR folding for state compression
- History mixing for improved diffusion
- Configurable rules and evolution steps

### Blockchain Implementation
- Proof-of-Work consensus mechanism
- Dual hash mode support (SHA-256 / AC_HASH)
- Dynamic hash mode switching
- Block validation and chain integrity verification
- Adjustable difficulty levels

### Analysis Tools
- Performance benchmarking suite
- Avalanche effect testing
- Bit distribution analysis
- Multi-rule comparison framework

---

## 📁 Project Structure

```
blockchain-ca/
├── include/              # Header files
│   ├── cellular_automaton.h
│   ├── ac_hash.h
│   ├── block.h
│   ├── block_pow.h
│   ├── blockchain_pow.h
│   ├── pow.h
│   └── utils.h
├── src/                  # Implementation files
│   ├── cellular_automaton.cpp
│   ├── ac_hash.cpp
│   ├── block_pow.cpp
│   ├── blockchain_pow.cpp
│   ├── pow.cpp
│   └── utils.cpp
├── tests/                # Test suite
│   ├── test_1.cpp        # CA implementation
│   ├── test_2.cpp        # Hash function
│   ├── test_3.cpp        # Blockchain integration
│   ├── test_4_benchmark.cpp  # Performance
│   ├── test_5.cpp        # Avalanche effect
│   ├── test_6.cpp        # Bit distribution
│   ├── test_7.cpp        # Rule comparison
│   └── run_tests.sh      # Automated test runner
├── build/                # Compiled executables (generated)
├── Makefile              # Build automation
└── README.md             # This file
```

---


## Prerequisites

**Required:**
- C++ compiler with C++11 support (GCC/Clang/MSVC)
- OpenSSL library (for SHA-256 comparison)

**For Windows (MSYS2/MinGW):**
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl
```

**For Linux (Ubuntu/Debian):**
```bash
sudo apt-get install build-essential libssl-dev
```

**For macOS:**
```bash
brew install openssl
```

## Installation

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/blockchain-ca.git
cd blockchain-ca
```

2. **Build the project**
```bash
# Using Makefile (recommended)
make all

# Or build specific tests
make test_1
make test_5
```

3. **Run all tests**
```bash
# Automated script (MSYS2/Linux/macOS)
chmod +x tests/run_tests.sh
./tests/run_tests.sh

# Or using Make
make test
```

## Quick Start Example

```cpp
#include "ac_hash.h"
#include <iostream>

int main() {
    // Generate a hash using Rule 30 with 128 evolution steps
    std::string hash = ac_hash("Hello, Blockchain!", 30, 128);
    std::cout << "Hash: " << hash << std::endl;
    return 0;
}
```

**Compile:**
```bash
g++ -std=c++11 -I./include example.cpp src/cellular_automaton.cpp src/ac_hash.cpp -o example
```

---

## 🔬 Key Findings

### Cryptographic Quality (AC_HASH with Rule 30)

| Metric | Result | Evaluation |
|--------|--------|------------|
| **Avalanche Effect** | 47.86% | Excellent (target: 50%) |
| **Bit Distribution** | 49.82% | Excellent (target: 50%) |
| **Hash Length** | 256 bits | Standard |
| **Reproducibility** | 100% | Deterministic |
| **Collision Resistance** | Not formally proven | Experimental |

### Performance Comparison (Difficulty 2, 10 blocks average)

| Hash Method | Avg Time per Block | Iterations | Speed Ratio |
|-------------|-------------------|------------|-------------|
| **SHA-256** | 3.0 ms | 232 | 1x (baseline) |
| **AC_HASH** | 403.9 ms | 228 | **134x slower** |

**Key Observations:**
- AC_HASH requires similar iteration counts but is significantly slower per hash
- Performance gap increases with difficulty (up to 320x at difficulty 4)
- SHA-256 benefits from decades of hardware optimization

### Rule Comparison (Rule 30 vs 90 vs 110)

| Rule | Speed | Avalanche | Balance | Recommendation |
|------|-------|-----------|---------|----------------|
| **30** | Fastest | 45.70% | 50.39% | **Best for hashing** |
| **90** | Slowest | 22.27% | 50.00% | Too predictable |
| **110** | Medium | 50.39% | 55.08% | Acceptable |

**Conclusion:** Rule 30 offers the optimal balance of speed, avalanche effect, and bit distribution.

---

## 💡 Usage Examples

### Creating a Blockchain with AC_HASH

```cpp
#include "blockchain_pow.h"

int main() {
    // Create blockchain with AC_HASH (difficulty 2, rule 30, 128 steps)
    BlockchainPow blockchain(2, AC_HASH_MODE, 30, 128);
    
    // Add blocks with transactions
    std::vector<std::string> transactions = {
        "Alice->Bob: 50 BTC",
        "Bob->Charlie: 30 BTC"
    };
    blockchain.addBlock(transactions);
    
    // Validate chain integrity
    bool valid = blockchain.isChainValid();
    std::cout << "Chain valid: " << (valid ? "YES" : "NO") << std::endl;
    
    return 0;
}
```

### Switching Hash Modes

```cpp
// Start with SHA-256
BlockchainPow chain(2, SHA256_MODE);
chain.addBlock({"Transaction 1"});

// Switch to AC_HASH
chain.setHashMode(AC_HASH_MODE, 30, 128);
chain.addBlock({"Transaction 2"});

// Both blocks remain valid!
chain.displayChain();
```

### Custom Hash Configuration

```cpp
// Use Rule 110 with 256 evolution steps
std::string hash = ac_hash("My data", 110, 256);

// Compare different rules
std::string hash_r30 = ac_hash("Test", 30, 128);
std::string hash_r90 = ac_hash("Test", 90, 128);
std::string hash_r110 = ac_hash("Test", 110, 128);
```

---

## 🧪 Testing

### Test Suite Overview

| Test | Description | Focus |
|------|-------------|-------|
| **test_1** | CA Implementation | Basic functionality |
| **test_2** | Hash Function | Conversion & hashing |
| **test_3** | Blockchain Integration | Mining & validation |
| **test_4** | Performance Benchmark | Speed comparison |
| **test_5** | Avalanche Effect | Bit sensitivity |
| **test_6** | Bit Distribution | Statistical quality |
| **test_7** | Rule Comparison | Multi-rule analysis |

### Running Tests

**All tests:**
```bash
./tests/run_tests.sh    # Bash script
make test               # Makefile
```

**Individual tests:**
```bash
make test_5             # Run avalanche test only
./build/test_5.exe      # Direct execution
```

**Manual compilation:**
```bash
# Test 2 (AC Hash)
g++ -std=c++11 -I./include tests/test_2.cpp \
    src/cellular_automaton.cpp src/ac_hash.cpp \
    -o build/test_2.exe

# Test 3 (Blockchain)
g++ -std=c++11 -I./include tests/test_3.cpp \
    src/*.cpp -lssl -lcrypto -o build/test_3.exe
```

---

## 📊 Analysis & Discussion

### Advantages of CA-Based Hashing

1. **ASIC Resistance Potential**
   - Sequential evolution prevents massive parallelization
   - Different from algebraic operations in SHA-256
   - Could promote mining decentralization

2. **Alternative Security Model**
   - Security from chaotic dynamical systems
   - Novel approach to cryptographic diffusion
   - Diversification of blockchain security

3. **Simplicity & Transparency**
   - Minimal code (simple bitwise operations)
   - Easy to audit and verify
   - No complex mathematical transformations

4. **Flexibility**
   - Adjustable parameters (rule, steps, size)
   - Extensible to 2D/3D automata
   - Potential for hybrid approaches

### Limitations & Challenges

1. **Performance Overhead**
   - 55-320x slower than SHA-256
   - Not suitable for high-throughput blockchains
   - Each hash requires 128+ sequential evolutions

2. **Limited Cryptanalysis**
   - No formal security proofs
   - Insufficient peer review compared to SHA-256
   - Unknown collision resistance properties

3. **Predictability Risks**
   - Some rules (e.g., Rule 90) too structured
   - Potential pattern emergence over time
   - Requires careful rule selection

4. **No Hardware Optimization**
   - No CPU instruction support
   - No ASIC/FPGA implementations
   - Limited performance optimization potential

### Proposed Improvements

1. **Hybrid Approach**
   ```
   final_hash = SHA256(ac_hash(input) || input)
   ```
   Combines ASIC resistance with proven security.

2. **Dynamic Rule Selection**
   ```cpp
   uint32_t rule = 30 + (block_height % 3) * 40;  // 30, 70, 110
   ```
   Prevents rule-specific optimizations.

3. **Multi-Rule Cascade**
   ```cpp
   ca.set_rule(30);  ca.evolve_steps(50);
   ca.set_rule(110); ca.evolve_steps(50);
   ca.set_rule(90);  ca.evolve_steps(28);
   ```
   Leverages properties of multiple rules.

4. **Adaptive Parameters**
   - Variable neighborhood radius
   - Input-dependent step count
   - Context-aware salt injection

---

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

**How to Contribute ?**

1. **Fork the repository**
2. **Create a feature branch**
   ```bash
   git checkout -b feature/amazing-feature
   ```
3. **Make your changes**
4. **Run tests**
   ```bash
   make test
   ```
5. **Commit your changes**
   ```bash
   git commit -m "Add amazing feature"
   ```
6. **Push to your fork**
   ```bash
   git push origin feature/amazing-feature
   ```
7. **Open a Pull Request**

### Areas for Contribution

- 🔬 **Cryptanalysis**: Formal security analysis of ac_hash
- ⚡ **Optimization**: Performance improvements for CA evolution
- 🧮 **Algorithms**: Alternative extraction/compression methods, haching/cryptographic algorithms integration
- 📊 **Analysis**: Additional statistical tests and benchmarks
- 📚 **Documentation**: Tutorials, examples, and explanations
- 🐛 **Bug Fixes**: Issue reports and fixes

<!-- ### Code Style

- Follow existing code style (K&R braces, 4-space indentation)
- Document public functions with comments
- Add tests for new features
- Keep commits atomic and well-described -->

---

## 📖 References

### Cellular Automata
- Wolfram, S. (1983). *Statistical mechanics of cellular automata*
- Wolfram, S. (2002). *A New Kind of Science*
- Rule 30: Used in Mathematica's random number generator

### Cryptographic Hashing
- NIST FIPS 180-4: Secure Hash Standard (SHA-256)
- Preneel, B. (2003). *Cryptographic Hash Functions*

### Blockchain Technology
- Nakamoto, S. (2008). *Bitcoin: A Peer-to-Peer Electronic Cash System*
- Antonopoulos, A. (2017). *Mastering Bitcoin*

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 👥 Authors

**Imad El Maftouhi**

- Project: Blockchain with Cellular Automaton Hash Function
- Institution: Master IASD, Department of Computer Science, FST Tanger.
- Affiliation: Master Artificial Intelligence & Data Science 2024/2025

---

## 🙏 Acknowledgments

- Prof. Ikram Ben Abdelouahab for course instruction for project guidance.

---

<div align="center">

**⭐ Star this repository if you find it interesting!**

Made with ❤️ for blockchain research and education

</div>