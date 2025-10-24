#include "pow.h"
#include "utils.h"
#include "ac_hash.h"
#include <sstream>

//compute hash based on mode
std::string ProofOfWork::computeHash(
    const std::string& data, HashMode mode, 
    uint32_t rule, 
    size_t steps) {
    if (mode == SHA256_MODE) {
        return sha256(data);
    } else {
        return ac_hash(data, rule, steps);
    }
}

//SHA256 mining (backward compatibility)
std::string ProofOfWork::mineBlock(const std::string& data, const std::string& previousHash, 
                                  int difficulty, int& nonce) {
    std::string target(difficulty, '0');
    std::string blockData = data + previousHash + std::to_string(nonce);
    std::string hash = sha256(blockData);

    while (hash.substr(0, difficulty) != target) {
        nonce++;
        blockData = data + previousHash + std::to_string(nonce);
        hash = sha256(blockData);
    }
    return hash;
}

//Mining with hash mode selection
std::string ProofOfWork::mineBlock(const std::string& data, const std::string& previousHash, 
                                  int difficulty, int& nonce, HashMode mode, 
                                  uint32_t rule, size_t steps) {
    std::string target(difficulty, '0');
    nonce = 0;
    
    std::string blockData = data + previousHash + std::to_string(nonce);
    std::string hash = computeHash(blockData, mode, rule, steps);

    while (hash.substr(0, difficulty) != target) {
        nonce++;
        blockData = data + previousHash + std::to_string(nonce);
        hash = computeHash(blockData, mode, rule, steps);
    }
    
    return hash;
}

//SHA256 verification
bool ProofOfWork::verifyBlock(const std::string& data, const std::string& previousHash, 
                             const std::string& hash, int difficulty, int nonce) {
    std::string target(difficulty, '0');
    std::string blockData = data + previousHash + std::to_string(nonce);
    std::string calculatedHash = sha256(blockData);
    return calculatedHash == hash && calculatedHash.substr(0, difficulty) == target;
}

//Verification with hash mode selection
bool ProofOfWork::verifyBlock(const std::string& data, const std::string& previousHash, 
                             const std::string& hash, int difficulty, int nonce, 
                             HashMode mode, uint32_t rule, size_t steps) {
    std::string target(difficulty, '0');
    std::string blockData = data + previousHash + std::to_string(nonce);
    std::string calculatedHash = computeHash(blockData, mode, rule, steps);
    return calculatedHash == hash && calculatedHash.substr(0, difficulty) == target;
}