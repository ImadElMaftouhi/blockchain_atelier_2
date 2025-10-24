/**
 * # Test 6 - Analyse la distribution des bits produits par ac_hash :
 * 6.1. Calcule le pourcentage de bits à 1 sur un échantillon d’au moins 105 bits.
 * 6.2. Indique si la distribution est équilibrée (≈50 % de 1).
 * 
 * # Compile and run
 * g++ -std=c++11 -I./include src/cellular_automaton.cpp src/ac_hash.cpp tests/test_6.cpp -o ./build/test_6.exe ; .\build\test_6.exe
 */

#include "ac_hash.h"
#include <iostream>
#include <iomanip>

int main() {
    std::string input = "Hello, world!";
    uint32_t rule = 30;
    size_t steps = 1000;

    std::string hash = ac_hash(input, rule, steps);

    std::cout << "Hash: " << hash << std::endl;

    int count = 0;
    for (char c : hash) {
        if (c == '1') {
            count++;
        }
    }

    double percentage = static_cast<double>(count) / hash.length() * 100;

    std::cout << "Percentage of 1 bits: " << std::fixed << std::setprecision(2) << percentage << "%" << std::endl;

    return 0;
}

