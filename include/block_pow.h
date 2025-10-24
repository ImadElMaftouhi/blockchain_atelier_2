#ifndef BLOCK_POW_H
#define BLOCK_POW_H

#include "block.h"
#include "utils.h"
#include <string>
#include <cstdint>

class BlockPow : public Block {
private:
    int index;
    std::string previousHash;
    std::string hash;
    std::string data;
    int nonce;
    int difficulty;
    HashMode hashMode;      //Hash mode selection
    uint32_t rule;          //CA rule (for AC_HASH mode)
    size_t steps;           //CA steps (for AC_HASH mode)

public:
    //constructor
    BlockPow(int idx, const std::string& prevHash, const std::string& h, 
             const std::string& d, int n, int diff, 
             HashMode mode = SHA256_MODE, uint32_t r = 30, size_t s = 128);
    
    ~BlockPow() override;
    
    std::string getHash() const override;
    std::string getPreviousHash() const override;
    int getIndex() const override;
    std::string calculateHash() const override;
    void display() const override;
    
    std::string getData() const;
    int getNonce() const;
    int getDifficulty() const;
    //
    HashMode getHashMode() const;
    uint32_t getRule() const;
    size_t getSteps() const;
};

#endif