#include "cellular_automaton.h"
#include <iostream>

int main() {
    // Create CA with Rule 30, size 9
    CellularAutomaton ca(9, 30);
    
    // Initialize with single center cell
    ca.init_single_center();
    
    std::cout << "Generation 0: ";
    ca.print_state();
    
    // Evolve 3 generations
    for (int i = 1; i <= 3; i++) {
        ca.evolve();
        std::cout << "Generation " << i << ": ";
        ca.print_state();
    }
    
    return 0;
}



/*
**Expected output for Rule 30:**
```
Generation 0:     █    
Generation 1:    ███   
Generation 2:   ██ ██  
Generation 3:  ██   ██
```
*/