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
private:
    int apply_rule(int left, int center, int right) const;//apply rule to a 3-cell neighborhood
};


#endif

/*
- grid wraps around (left edge connects to right edge)
- 

*/