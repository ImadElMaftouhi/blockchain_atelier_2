#include "ac_hash.h"
#include "cellular_automaton.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

std::vector<int> string_to_bits(const std::string& input) {
    std::vector<int> bits;
    bits.reserve(input.length() * 8);
    for (unsigned char c : input) {
        for (int i = 7; i >= 0; i--) {
            bits.push_back((c >> i) & 1);
        }
    }
    return bits;
}

std::string bits_to_hex(const std::vector<int>& bits) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    for (size_t i = 0; i < bits.size(); i += 4) {
        int nibble = 0;
        for (int j = 0; j < 4 && (i + j) < bits.size(); j++) {
            nibble = (nibble << 1) | bits[i + j];
        }
        ss << std::setw(1) << nibble;
    }
    
    return ss.str();
}

std::vector<int> extract_hash_bits(const std::vector<int>& state, 
                                    const std::vector<std::vector<int>>& history) {
    std::vector<int> hash_bits(256, 0);
    for (size_t i = 0; i < state.size() && i < 256; i++) {
        hash_bits[i] = state[i];
    }
    if (state.size() > 256) {
        for (size_t i = 256; i < state.size(); i++) {
            hash_bits[i % 256] ^= state[i];
        }
    }
    size_t sample_interval = std::max(size_t(1), history.size() / 32);
    for (size_t h = 0; h < history.size(); h += sample_interval) {
        const auto& hist_state = history[h];
        for (size_t i = 0; i < hist_state.size(); i++) {
            size_t hash_idx = (i + h * 7) % 256;
            hash_bits[hash_idx] ^= hist_state[i];
        }
    }
    if (state.size() < 256) {
        size_t repetitions = 256 / state.size() + 1;
        for (size_t rep = 1; rep < repetitions; rep++) {
            for (size_t i = 0; i < state.size(); i++) {
                size_t idx = (rep * state.size() + i) % 256;
                hash_bits[idx] ^= state[i];
            }
        }
    }
    
    return hash_bits;
}

std::string ac_hash(const std::string& input, uint32_t rule, size_t steps) {
    std::vector<int> input_bits = string_to_bits(input);
    size_t ca_size = std::max(size_t(256), input_bits.size());
    while (input_bits.size() < ca_size) {
        input_bits.push_back(input_bits.size() % 2);
    }
    CellularAutomaton ca(ca_size, rule);
    ca.init_state(input_bits);
    
    std::vector<std::vector<int>> history;
    history.push_back(ca.get_state());
    
    for (size_t i = 0; i < steps; i++) {
        ca.evolve();
        if (i % (steps / 16 + 1) == 0) {
            history.push_back(ca.get_state());
        }
    }
    
    std::vector<int> final_state = ca.get_state();
    history.push_back(final_state);
    
    std::vector<int> hash_bits = extract_hash_bits(final_state, history);
    
    return bits_to_hex(hash_bits);
}