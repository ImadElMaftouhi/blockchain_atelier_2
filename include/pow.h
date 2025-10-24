#ifndef POW_H
#define POW_H

#include <string>
#include <cstdint>
#include "utils.h"

class ProofOfWork {
public:
    //SHA256 mining
    static std::string mineBlock(const std::string& data, const std::string& previousHash, 
                                int difficulty, int& nonce);
    
    //mine with hash mode selection
    static std::string mineBlock(const std::string& data, const std::string& previousHash, 
                                int difficulty, int& nonce, HashMode mode, 
                                uint32_t rule = 30, size_t steps = 128);
    
    //SHA256 verification
    static bool verifyBlock(const std::string& data, const std::string& previousHash, 
                          const std::string& hash, int difficulty, int nonce);
    
    //Verification with hash mode selection
    static bool verifyBlock(const std::string& data, const std::string& previousHash, 
                          const std::string& hash, int difficulty, int nonce, 
                          HashMode mode, uint32_t rule = 30, size_t steps = 128);

private:
    //Compute hash based on mode
    static std::string computeHash(const std::string& data, HashMode mode, 
                                  uint32_t rule, size_t steps);
};

#endif