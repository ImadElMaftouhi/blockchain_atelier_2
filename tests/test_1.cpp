/**
/**
 * Test 1: Basic cellular automaton test
 * Compile and run 
 * 
 * ```bash
 * g++ -I./include tests/test_1.cpp src/cellular_automaton.cpp -o ./build/test_1.exe ; ./build/test_1.exe
 * ```
 * 
 */

#include "cellular_automaton.h"
#include <iostream>

int main() {
    CellularAutomaton ca(100, 30);//Rule 30 & size 9
    ca.init_single_center();
    std::cout << "Generation 0: ";
    ca.print_state();
    
    // Evolve 3 generations
    for (int i = 1; i <= 62 ; i++) {
        ca.evolve();
        std::cout << "Generation " << i << ": ";
        ca.print_state();
    }

    // // Evolve 3 generations
    // std::string input;
    // while (true) {
    //     ca.evolve();
    //     std::cout << "Generation " << (i++) << ": ";
    //     ca.print_state();
    //     std::cout << "Press 'q' to quit or any other key to continue: ";
    //     std::getline(std::cin, input);
    //     if (input == "q") break;
    // }
    
    return 0;
}