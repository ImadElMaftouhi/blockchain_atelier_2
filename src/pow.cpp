#include "pow.h"
#include "utils.h"
#include "ac_hash.h"
#include <sstream>

/**
 * Computes a hash based on the given data and hash mode.
 * If the mode is SHA256_MODE, it uses the sha256 function
 * to compute the hash. If the mode is AC_HASH_MODE, it uses
 * the ac_hash function with the given rule and steps.
 * @param data The data to be hashed
 * @param mode The hash mode to use (SHA256_MODE or AC_HASH_MODE)
 * @param rule The CA rule to use (only for AC_HASH_MODE)
 * @param steps The number of steps to evolve the CA (only for AC_HASH_MODE)
 * @return The computed hash
 */
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

/**
 * Mines a new block using the given data, previous hash, and difficulty.
 * @param data The data to be added to the block
 * @param previousHash The hash of the previous block in the blockchain
 * @param difficulty The difficulty of the blockchain
 * @param nonce The nonce value to be used in the block
 * @return The newly mined block hash
 */

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

/**
 * Mines a block by incrementing the nonce until a hash with the given difficulty is found.
 * @param data The data to be hashed.
 * @param previousHash The previous hash in the blockchain.
 * @param difficulty The difficulty of the blockchain.
 * @param nonce The nonce for the block.
 * @param mode The hash mode of the blockchain (SHA256_MODE or AC_HASH_MODE).
 * @param rule The CA rule (for AC_HASH_MODE).
 * @param steps The CA steps (for AC_HASH_MODE).
 * @return The hash of the block.
 */
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