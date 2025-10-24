/**
 * test 4 - Analyse the Avalanche Effect of CA
 * # objectif
 * - Calculate the average percentage of bit differences between the hashes of two messages that differ by only one bit.
 * - Provide the numerical result and your test code.
 * 
 * # Steps
 * Flips 1 bit in 100 different messages
 * Compares hashes before/after flip
 * Calculates % of bits changed
 * Good avalanche: ~50% bits change
 * 
 * 
 * # Compile and run
 * g++ -std=c++11 -I./include src/cellular_automaton.cpp src/ac_hash.cpp tests/test_5.cpp -o ./build/test_5.exe ; .\build\test_5.exe
 * 
 */

#include "ac_hash.h"
#include <iostream>
#include <iomanip>

/**
 * Flips one bit in a given string
 * @param input the string to flip a bit in
 * @param bitPos the position of the bit to flip (0-based)
 * @return a new string with the bit flipped
 * 
 * This function will return the original string if the bitPos is out of range
 * of the string
 */
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

void testAvalancheEffect(int numTests = 100) {
    std::cout << "\n==========================================================\n";
    std::cout << "=        EXERCISE 5: AVALANCHE EFFECT ANALYSIS             =\n";
    std::cout << "============================================================\n";
    
    std::cout << "\nTesting with " << numTests << " different messages...\n";
    
    int totalDiffBits = 0;
    int totalBits = 256;
    
    for (int i = 0; i < numTests; i++) {

        std::string message = "Test message number " + std::to_string(i);
        std::string flippedMessage = flipBit(message, i % (message.length() * 8));
        std::string hash1 = ac_hash(message, 30, 128);
        std::string hash2 = ac_hash(flippedMessage, 30, 128);
        
        int diffBits = countDifferentBits(hash1, hash2);
        totalDiffBits += diffBits;
    }
    
    double avgDiffBits = static_cast<double>(totalDiffBits) / numTests;
    double percentage = (avgDiffBits / totalBits) * 100.0;
    
    std::cout << "\n--- Results ---" << std::endl;
    std::cout << "Average bits changed: " << std::fixed << std::setprecision(2) 
              << avgDiffBits << " / " << totalBits << std::endl;
    std::cout << "Percentage: " << std::fixed << std::setprecision(2) 
              << percentage << "%" << std::endl;
    
    std::cout << "\n--- Analysis ---" << std::endl;
    if (percentage >= 45.0 && percentage <= 55.0) {
        std::cout << "GOOD: Close to ideal 50% (strong avalanche effect)" << std::endl;
    } else if (percentage >= 40.0 && percentage <= 60.0) {
        std::cout << "ACCEPTABLE: Within 40-60% range" << std::endl;
    } else {
        std::cout << "WEAK: Avalanche effect needs improvement" << std::endl;
    }
}

int main() {
    testAvalancheEffect(100);
    std::cout << std::endl;
    return 0;
}