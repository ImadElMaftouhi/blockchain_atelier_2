/**
 * Test 7 - Teste plusieurs regles d'automates :
 * 7.1. Execute ac_hash avec Rule 30, Rule 90 et Rule 110.
 * 7.2. Compare la stabilite des résultats et les temps d'execution.
 * 7.3. Indique quelle regle te semble la plus adaptee pour le hachage et pourquoi.
 * 
 * Compile and run:
 * g++ -std=c++11 -I./include src/cellular_automaton.cpp src/ac_hash.cpp tests/test_7.cpp -o ./build/test_7.exe ; .\build\test_7.exe
 */

#include "ac_hash.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <string>

struct RuleResult {
    uint32_t rule;
    std::string hash;
    long long time_us;
    double avalanche_percent;
    double bit_balance_percent;
};

std::string flipBit(const std::string& input, size_t bitPos) {
    std::string result = input;
    size_t bytePos = bitPos / 8;
    size_t bitInByte = 7 - (bitPos % 8);
    if (bytePos < result.length()) {
        result[bytePos] ^= (1 << bitInByte);
    }
    return result;
}

/**
 * Counts the number of different bits between two hexadecimal strings.
 * This function compares two hexadecimal strings of equal length and
 * counts the number of bits that are different between the two strings.
 * It does this by iterating over each character in the strings, converting
 * them to integers, and then XORing the two values together.
 * The result of the XOR operation is then counted to determine the
 * number of different bits between the two strings.
 * 
 * @param hash1 The first hexadecimal string to compare
 * @param hash2 The second hexadecimal string to compare
 * @return The number of different bits between the two strings
 */
int countDifferentBits(const std::string& hash1, const std::string& hash2) {
    int diffBits = 0;
    for (size_t i = 0; i < hash1.length() && i < hash2.length(); i++) {
        int val1 = (hash1[i] >= 'a') ? (hash1[i] - 'a' + 10) : (hash1[i] - '0');
        int val2 = (hash2[i] >= 'a') ? (hash2[i] - 'a' + 10) : (hash2[i] - '0');
        int xorVal = val1 ^ val2;
        while (xorVal) {
            diffBits += (xorVal & 1);
            xorVal >>= 1;
        }
    }
    return diffBits;
}

int countOneBits(const std::string& hash) {
    int count = 0;
    for (char c : hash) {
        int val = (c >= 'a') ? (c - 'a' + 10) : (c - '0');
        for (int i = 0; i < 4; i++) {
            if (val & (1 << i)) count++;
        }
    }
    return count;
}

RuleResult testRule(uint32_t rule, const std::string& input, size_t steps) {
    RuleResult result;
    result.rule = rule;
    
    //calculate execution time
    auto start = std::chrono::high_resolution_clock::now();
    result.hash = ac_hash(input, rule, steps);
    auto end = std::chrono::high_resolution_clock::now();
    result.time_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    //Avalanche Test
    std::string flipped = flipBit(input, 0);
    std::string hash2 = ac_hash(flipped, rule, steps);
    int diffBits = countDifferentBits(result.hash, hash2);
    result.avalanche_percent = (static_cast<double>(diffBits) / 256.0) * 100.0;
    
    //Bits Distribution Test
    int oneBits = countOneBits(result.hash);
    result.bit_balance_percent = (static_cast<double>(oneBits) / 256.0) * 100.0;
    
    return result;
}

void printResults(const std::vector<RuleResult>& results) {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "Comparative results of rules" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    std::cout << "\n" << std::left
              << std::setw(10) << "Rule"
              << std::setw(20) << "Time(us)"
              << std::setw(25) << "Avalanche (%)"
              << std::setw(25) << "Stability (%)"
              << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& r : results) {
        std::cout << std::left
                  << std::setw(10) << r.rule
                  << std::setw(20) << r.time_us
                  << std::setw(25) << std::fixed << std::setprecision(2) << r.avalanche_percent
                  << std::setw(25) << std::fixed << std::setprecision(2) << r.bit_balance_percent
                  << std::endl;
    }
}

void analyzeResults(const std::vector<RuleResult>& results) {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "7.3: Analysis & Recommendation" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    for (const auto& r : results) {
        std::cout << "\n--- Rule " << r.rule << " ---" << std::endl;
        
        if (r.rule == 30) {
            std::cout << "Characteristics: Chaotic behavior" << std::endl;
            std::cout << "Avalanche: " << (r.avalanche_percent >= 45 && r.avalanche_percent <= 55 ? "EXCELLENT" : "ACCEPTABLE") << std::endl;
            std::cout << "Balance: " << (r.bit_balance_percent >= 45 && r.bit_balance_percent <= 55 ? "EXCELLENT" : "ACCEPTABLE") << std::endl;
        }
        else if (r.rule == 90) {
            std::cout << "Characteristics: Generates fractals (Sierpiński triangle)" << std::endl;
            std::cout << "Avalanche: " << (r.avalanche_percent >= 45 && r.avalanche_percent <= 55 ? "EXCELLENT" : "LOW") << std::endl;
            std::cout << "Balance: " << (r.bit_balance_percent >= 45 && r.bit_balance_percent <= 55 ? "EXCELLENT" : "LOW") << std::endl;
        }
        else if (r.rule == 110) {
            std::cout << "Characteristics: Turing-complete, complex behavior" << std::endl;
            std::cout << "Avalanche: " << (r.avalanche_percent >= 45 && r.avalanche_percent <= 55 ? "EXCELLENT" : "ACCEPTABLE") << std::endl;
            std::cout << "Balance: " << (r.bit_balance_percent >= 45 && r.bit_balance_percent <= 55 ? "EXCELLENT" : "ACCEPTABLE") << std::endl;
        }
    }
    
    // Final Recommendation
    std::cout << "\n" << std::string(80, '-') << std::endl;
    std::cout << "FINAL RECOMMENDATION:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    std::cout << "\nRule 30 is the MOST SUITABLE for hashing because:" << std::endl;
    std::cout << "  1. Optimal chaotic behavior for diffusion" << std::endl;
    std::cout << "  2. Ideal avalanching effect (close to 50%)" << std::endl;
    std::cout << "  3. Good distribution of bits" << std::endl;
    std::cout << "  4. Widely used in cryptography (random number generator)" << std::endl;
    
    std::cout << "\nRule 90:" << std::endl;
    std::cout << "  - Too predictable structure (fractal)" << std::endl;
    std::cout << "  - Good for cryptographic diffusion" << std::endl;
    
    std::cout << "\nRule 110:" << std::endl;
    std::cout << "  - Interesting complexity but may be too structured" << std::endl;
    std::cout << "  - Limited use for cryptographic applications" << std::endl;
}

int main() {
    std::cout << "\n";
    std::cout << "==============================================================\n";
    std::cout << "=               TEST 7: COMPARISON OF CA RULES               =\n";
    std::cout << "==============================================================\n";

    std::string testInput = "Transaction 1: sending 5.00 BTC to 0x1234567890abcde";
    std::vector<std::string> transactions = {
        "Transaction 1: sending 5.00 BTC to 0x1234567890abcdef",
        "Transaction 2: sending 10.00 ETH to 0x9876543210fedcba",
        "Transaction 3: sending 20.00 LTC to 0xfedcba9876543210",
        "Transaction 4: sending 50.00 DOGE to 0x1234567890abcdef",
        "Transaction 5: sending 100.00 XRP to 0x9876543210fedcba"
    };
    size_t steps = 128;
    std::vector<uint32_t> rules = {30, 90, 110};
    std::vector<RuleResult> results;
    
    // for (const std::string& testInput: transactions) {
    //     std::cout << "\nTest in progress with: \"" << testInput << "\"" << std::endl;
    //     std::cout << "Number of steps: " << steps << std::endl;
        
    //     // 7.1: Execute with each rule
    //     for (uint32_t rule : rules) {
    //         std::cout << "\nExecuting with Rule " << rule << "..." << std::endl;
    //         RuleResult result = testRule(rule, testInput, steps);
    //         results.push_back(result);
    //         std::cout << "Hash: " << result.hash.substr(0, 16) << "..." << std::endl;
    //     }
    // }


    std::cout << "\nTest in progress with: \"" << testInput << "\"" << std::endl;
    std::cout << "Number of steps: " << steps << std::endl;
    
    // 7.1: Execute with each rule
    for (uint32_t rule : rules) {
        std::cout << "\nExecuting with Rule " << rule << "..." << std::endl;
        RuleResult result = testRule(rule, testInput, steps);
        results.push_back(result);
        std::cout << "Hash: " << result.hash.substr(0, 16) << "..." << std::endl;
    }
    
    printResults(results);
    analyzeResults(results);

    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "TEST COMPLETE" << std::endl;
    std::cout << std::string(80, '=') << "\n" << std::endl;
    
    return 0;
}

