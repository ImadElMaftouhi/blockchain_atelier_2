#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <chrono>
#include <ctime>
#include <openssl/sha.h>
#include <ac_hash.h>

enum HashMode {
    SHA256_MODE,
    AC_HASH_MODE
};

std::string sha256(const std::string& input);
std::string ac_hash(const std::string& input, uint32_t rule, size_t steps);
std::string getCurrentTime();

// Template must be defined in header. Return microseconds for better granularity.
template<typename F>
long long measureTime(F func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Helper to convert microseconds to milliseconds (rounded)
inline long long microsToMillis(long long micros) {
    return (micros + 500) / 1000;
}

inline std::string hashModeToString(HashMode mode){
    return (mode == SHA256_MODE) ? "SHA-256" : "AC HASH";
}

#endif