#include "ac_hash.h"
#include "cellular_automaton.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

/**
 * Converts a given string to a vector of bits (0 or 1) based on ASCII representation
 * of each character in the string.
 * 
 * @param input The string to be converted to bits
 * @return A vector of bits (0 or 1) which represents the input string
 */
std::vector<int> string_to_bits(const std::string& input) {
    std::vector<int> bits;
    bits.reserve(input.length() * 8); //pre-allocate memory for the vector
    for (unsigned char c : input) {
        for (int i = 7; i >= 0; i--) {
            bits.push_back((c >> i) & 1);
        }
    }
    //cout << "bits :" << bits << endl;
    return bits;
}

/**
 * Converts a vector of bits (0 or 1) to a hexadecimal string representation
 * of the given bits.
 * 
 * @param bits The vector of bits to be converted to a hexadecimal string
 * @return A string which represents the given bits in hexadecimal form
 */

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

/**
 * Extracts a 256-bit hash from the given state and history.
 * The hash is constructed by taking the first 256 bits of the state,
 * and then XORing the remaining bits of the state with the hash.
 * The history is then sampled at regular intervals, and each sampled
 * state is used to further modify the hash by XORing each bit of
 * the sampled state with the corresponding bit in the hash.
 * If the state is shorter than 256 bits, it is repeated to fill the
 * hash, with each repetition being XORed with the previous one.
 * 
 * @param state The state from which to extract the hash
 * @param history The history of states from which to sample
 * @return A 256-bit hash extracted from the given state and history
 */
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

/**
 * Computes a hash of the given input string using a cellular automaton
 * with the given rule and number of steps.
 * 
 * The input string is first converted to a bit vector, which is
 * then padded with alternating bits to a length of at least
 * 256 bits.
 * A cellular automaton is then initialized with the given rule and
 * padded bit vector as the initial state.
 * The automaton is then run for the given number of steps, with
 * snapshots of the state being taken at regular intervals.
 * The final state of the automaton is then combined with the
 * snapshot history to produce a 256-bit hash.
 * 
 * @param input the string to be hashed
 * @param rule the rule number of the cellular automaton to use
 * @param steps the number of steps to run the automaton for
 * @return a 256-bit hash of the input string as a hexadecimal string
 */
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