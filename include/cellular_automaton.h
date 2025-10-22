#ifndef CELLULAR_AUTOMATON_H
#define CELLULAR_AUTOMATON_H

#include <string>
#include <vector>
#include <cstdint>

class CellularAutomaton {
private:
    std::vector<int> state;
    uint32_t rule;
    size_t size;
public:
    CellularAutomaton(size_t grid_size, uint32_t rule_number);
    void init_state(const std::vector<int>& initial_state);//init a vector of bit
    void init_single_center(); //init state from a single center cell
    void evolve(); //evolve one generation
    void evolve_setps(size_t steps); //run multiple
    std::vector<int> get_state() const; 
    int get_cell(size_t index) const;
    void set_rule(uint32_t rule_number);
    uint32_t get_rule() const;
    void print_state();
    std::string state_to_string() const; //get state as binary string
    void reset(); //all zeros
private:
    int apply_rule(int left, int center, int right) const;//apply rule to a 3-cell neighborhood
};


#endif

/*
- grid wraps around (left edge connects to right edge)
- 

*/