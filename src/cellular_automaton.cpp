#include "cellular_automaton.h"
#include <iostream>
#include <stdexcept>


// Constructor
CellularAutomaton::CellularAutomaton(size_t grid_size, uint32_t rule_number)
    : size(grid_size), rule(rule_number) {
    if (rule >255) {
        throw std::invalid_argument("Rule number must be between 0 and 255");
    }
    state.resize(size,0);
}

// init state from a vector
void CellularAutomaton::init_state(const std::vector<int>& initial_state){
    if (initial_state.size() != size){
        throw std::invalid_argument("Initial state size must match grid size");
    }
    for (int val:initial_state){
        if (val != 0 && val != 1){
            throw std::invalid_argument("State values must be 0 or 1");
        }
    }
    state = initial_state;
}