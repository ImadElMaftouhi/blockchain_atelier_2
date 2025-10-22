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

//init state with single  center cell set to 1
void CellularAutomaton::init_single_center(){
    std::fill(state.begin(), state.end(), 0);
    state[size/2] = 1;
};

int CellularAutomaton::apply_rule(int left, int center, int right) const {
    //convert 3-cell neighborhood to a pattern index (0-7)
    int pattern = (left << 2) | (center << 1) | right;
    //printf("pattern : %d", pattern);
    return (rule >> pattern) & 1;
}


void CellularAutomaton::evolve(){
    std::vector<int> new_state(size);
    for (size_t i = 0; i < size; i++){
        int left = state[(i-1 + size) % size];
        int center = state[i];
        int right = state[(i+1)%size];
        new_state[i] = apply_rule(left,center,right);
    }
    state = new_state;
}

void CellularAutomaton::evolve_steps(size_t steps){
    for (size_t i=0;i<steps;i++){
        evolve();
    }
}

std::vector<int> CellularAutomaton::get_state() const {
    return state;
};

int CellularAutomaton::get_cell(size_t index) const {
    if (index >= size){
        throw std::out_of_range("Cell index out of range");
    }
    return state[index];
}

void CellularAutomaton::set_rule(uint32_t rule_number) {
    if (rule_number > 255){
        throw std::invalid_argument("Rule number must be between 0 and 255");
    }
    rule = rule_number;
}

uint32_t CellularAutomaton::get_rule() const{
    return rule;
}

void CellularAutomaton::print_state() const {
    for (int cell : state ) {
        std::cout << (cell ? '#' : ' ');
    }
    std::cout << std::endl;
}

std::string CellularAutomaton::state_to_string() const {
    std::string result;
    result.reserve(size);
    for ( int cell : state) {
        result += (cell ? "1" : "0");
    }
    return result;
}

void CellularAutomaton::reset() {
    std::fill(state.begin(), state.end(), 0);
}