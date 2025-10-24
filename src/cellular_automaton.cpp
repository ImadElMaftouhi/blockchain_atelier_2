#include "cellular_automaton.h"
#include <iostream>
#include <stdexcept>


//constructor
CellularAutomaton::CellularAutomaton(size_t grid_size, uint32_t rule_number)
    : size(grid_size), rule(rule_number) {
    if (rule >255) {
        throw std::invalid_argument("Rule number must be between 0 and 255");
    }
    state.resize(size,0);//resize to grid size with default value of 0
}

//init state from a vector
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

/**
 * Applies the CA rule to a 3-cell neighborhood
 *
 * The neighborhood is converted to a pattern index (0-7) by
 * shifting the left and center cells to the left and right
 * positions, respectively. The result is then obtained by
 * shifting the rule number to the right by the pattern index and
 * taking the least significant bit.
 *
 * @param left Left cell in the neighborhood
 * @param center Center cell in the neighborhood
 * @param right Right cell in the neighborhood
 * @return Result of applying the CA rule to the neighborhood
 */
int CellularAutomaton::apply_rule(int left, int center, int right) const {
    //convert 3-cell neighborhood to a pattern index (0-7)
    int pattern = (left << 2) | (center << 1) | right;
    //cout << "left: " << left << ", center: " << center << ", right: " << right << endl;
    //cout << "pattern : " << pattern << endl;
    //cout << "rule": << rule << endl;
    return (rule >> pattern) & 1;
}


/**
 * Evolves the CA by one generation.
 * 
 * This function applies the CA rule to each cell in the current state
 * and stores the result in a new state. The new state then replaces
 * the current state.
 */
void CellularAutomaton::evolve(){
    std::vector<int> new_state(size);
    for (size_t i = 0; i < size; i++){
        int left = state[(i-1 + size) % size];//wraps around
        int center = state[i];
        int right = state[(i+1)%size];//wraps around
        new_state[i] = apply_rule(left,center,right);
    }
    state = new_state;
}

void CellularAutomaton::evolve_steps(size_t steps){
    for (size_t i=0;i<steps;i++){
        evolve();
    }
}


/**
 * Returns the current state of the cellular automaton as a vector of
 * integers where each value represents the state of a cell (0 or 1).
 * The state is returned by value, so the returned vector is a copy of
 * the internal state.
 * @return The current state of the cellular automaton.
 */
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


/**
 * Prints the current state of the cellular automaton to the console.
 * Each cell is represented by a single character ('#' for a cell with a value of 1 and ' ' for a cell with a value of 0). The state is printed in a single line, with cells separated by spaces.
 * @note This function is for debugging purposes only and should not be used in production code.
 */
void CellularAutomaton::print_state() const {
    for (int cell : state ) {
        std::cout << (cell ? '#' : ' ');
    }
    std::cout << std::endl;
}
/**
 * Returns a string representation of the current state of the
 * cellular automaton. Each cell is represented by a single
 * character ('1' for a cell with a value of 1 and '0' for a
 * cell with a value of 0). The string is returned by value, so
 * the returned string is a copy of the internal state.
 * @return A string representation of the current state of the
 *          cellular automaton.
 */
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