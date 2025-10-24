#ifndef BLOCKCHAIN_POW_H
#define BLOCKCHAIN_POW_H

#include "block_pow.h"
#include "utils.h"
#include <vector>
#include <string>
#include <cstdint>

class BlockchainPow {
private:
    std::vector<BlockPow*> chain;
    int difficulty;
    HashMode hashMode;      //Default hash mode for the blockchain
    uint32_t rule;          //CA rule (for AC_HASH mode)
    size_t steps;           //CA steps (for AC_HASH mode)

public:
    // Constructor with hash mode selection
    BlockchainPow(int diff = 2, HashMode mode = SHA256_MODE, 
                  uint32_t r = 30, size_t s = 128);
    
    ~BlockchainPow();
    
    void addBlock(const std::vector<std::string>& transactions);
    bool isChainValid() const;
    void displayChain() const;
    void setDifficulty(int diff);
    void setHashMode(HashMode mode, uint32_t r = 30, size_t s = 128);
    std::string getLatestHash() const;
    
    //getters for hash configuration
    HashMode getHashMode() const;
    uint32_t getRule() const;
    size_t getSteps() const;
    const std::vector<BlockPow*>& getChain() const;//get the chain for analysis
};

#endif