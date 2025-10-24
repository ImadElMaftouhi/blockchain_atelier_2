/**
 * Test 6 - Analyse la distribution des bits produits par ac_hash :
 * 6.1. Calcule le pourcentage de bits à 1 sur un échantillon d'au moins 10^5 bits.
 * 6.2. Indique si la distribution est équilibrée (≈50 % de 1).
 * 
 * Compile and run:
 * g++ -std=c++11 -I./include src/cellular_automaton.cpp src/ac_hash.cpp tests/test_6.cpp -o ./build/test_6.exe ; .\build\test_6.exe
 */

#include "ac_hash.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

int countOneBits(const std::string& hexHash) {
    int count = 0;
    for (char c : hexHash) {
        int val = hexCharToInt(c);
        // Count bits in the 4-bit nibble
        for (int i = 0; i < 4; i++) {
            if (val & (1 << i)) {
                count++;
            }
        }
    }
    return count;
}

void testSingleHash(const std::string& input, uint32_t rule, size_t steps) {
    std::string hash = ac_hash(input, rule, steps);
    
    int oneBits = countOneBits(hash);
    int totalBits = hash.length() * 4; // Each hex char = 4 bits
    double percentage = (static_cast<double>(oneBits) / totalBits) * 100.0;
    
    std::cout << "Input: \"" << input << "\"" << std::endl;
    std::cout << "Hash: " << hash << std::endl;
    std::cout << "Bits set to 1: " << oneBits << " / " << totalBits << std::endl;
    std::cout << "Percentage: " << std::fixed << std::setprecision(2) << percentage << "%" << std::endl;
    std::cout << std::endl;
}

void testBitDistribution(int numSamples) {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "6.1: Analysis on sample of " << numSamples << " hashes" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    long long totalOneBits = 0;
    long long totalBits = 0;
    uint32_t rule = 30;
    size_t steps = 128;
    
    for (int i = 0; i < numSamples; i++) {
        std::string input = "Sample message number " + std::to_string(i);
        std::string hash = ac_hash(input, rule, steps);
        
        int oneBits = countOneBits(hash);
        totalOneBits += oneBits;
        totalBits += 256; // each hash is 256 bits
        
        if (i < 5 || i == numSamples - 1) {
            std::cout << "Hash #" << i << ": " << oneBits << " bits set to 1 (";
            std::cout << std::fixed << std::setprecision(2);
            std::cout << (oneBits / 256.0 * 100.0) << "%)" << std::endl;
        } else if (i == 5) {
            std::cout << "..." << std::endl;
        }
    }
    
    double percentage = (static_cast<double>(totalOneBits) / totalBits) * 100.0;
    
    std::cout << "\n" << std::string(70, '-') << std::endl;
    std::cout << "GLOBAL RESULTS:" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    std::cout << "Number of hashes generated: " << numSamples << std::endl;
    std::cout << "Total analyzed bits: " << totalBits << " bits" << std::endl;
    std::cout << "Total bits set to 1: " << totalOneBits << std::endl;
    std::cout << "Average percentage: " << std::fixed << std::setprecision(2) 
              << percentage << "%" << std::endl;
    
    // 6.2: evaluate the stability
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "6.2: Stability evaluation" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    double deviation = std::abs(percentage - 50.0);
    
    std::cout << "Ideal value: 50.00%" << std::endl;
    std::cout << "Obtained result: " << std::fixed << std::setprecision(2) 
              << percentage << "%" << std::endl;
    std::cout << "Deviation: " << std::fixed << std::setprecision(2) 
              << (percentage - 50.0) << "%" << std::endl;
    
    std::cout << "\nEVALUATION: ";
    if (deviation <= 1.0) {
        std::cout << "EXCELLENT - Very well-balanced distribution" << std::endl;
    } else if (deviation <= 2.0) {
        std::cout << "GOOD - Well-balanced distribution" << std::endl;
    } else if (deviation <= 5.0) {
        std::cout << "ACCEPTABLE - Slightly unbalanced distribution" << std::endl;
    } else {
        std::cout << "BAD - Unbalanced distribution" << std::endl;
    }
}

int main() {
    std::cout << "\n";
    std::cout << "==============================================================\n";
    std::cout << "=        TEST 6: BIT DISTRIBUTION (ac_hash)            =\n";
    std::cout << "==============================================================\n";
    
    std::cout << "\n--- Individual tests ---\n" << std::endl;
    
    testSingleHash("Hello, World!", 30, 128);
    testSingleHash("Blockchain", 30, 128);
    testSingleHash("Test message", 30, 128);
    
    // 6.1 & 6.2: Analyse statistique sur large échantillon
    // Pour avoir au moins 10^5 bits, on a besoin de: 100000 / 256 ≈ 391 hashs
    // On va générer 400 hashs pour être sûr
    int numSamples = 400;
    testBitDistribution(numSamples);
    
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "TEST TERMINATED" << std::endl;
    std::cout << std::string(70, '=') << "\n" << std::endl;
    
    return 0;
}

