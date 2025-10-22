#ifndef AC_H
#define AC_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>


/**
 * Compute a 256-bit hash using cellular automaton evolution
 * 
 * @param input - Input string to hash
 * @param rule - CA rule number 
 * @param steps - Number of evolution setps
 * @return Hexadecimal string representation of 256-bit hash
 */
std::string ac_hash(const std::string& input, uint32_t rule, size_t steps);


std::vector<int> string_to_bits(const std::string& inout);

std::string bits_to_hex(const std::vector<int>& bits);

std::vector<int> extract_hash_bits(const std::vector<int>& state, const std::vector<std::vector<int>>& history);

#endif 