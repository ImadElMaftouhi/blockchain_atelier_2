#include "blockchain_pow.h"
#include "utils.h"
#include "pow.h"
#include <iostream>
#include <chrono>

BlockchainPow::BlockchainPow(int diff, HashMode mode, uint32_t r, size_t s) 
    : difficulty(diff), hashMode(mode), rule(r), steps(s) {
    
    //genesis block
    std::string genesisData = "Genesis Block";
    int nonce = 0;
    std::string genesisHash = ProofOfWork::mineBlock(genesisData, "0", difficulty, nonce, 
                                                     hashMode, rule, steps);
    chain.push_back(new BlockPow(0, "0", genesisHash, genesisData, nonce, difficulty, 
                                 hashMode, rule, steps));
}

BlockchainPow::~BlockchainPow() {
    for (auto* block : chain) {
        delete block;
    }
}

void BlockchainPow::addBlock(const std::vector<std::string>& transactions) {
    //combine transactions into a single data string
    std::string data;
    for (const auto& tx : transactions) {
        data += tx + ";";
    }
    
    std::string prevHash = getLatestHash();
    int nonce = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string newHash = ProofOfWork::mineBlock(data, prevHash, difficulty, nonce, 
                                                 hashMode, rule, steps);
    auto end = std::chrono::high_resolution_clock::now();
    
    long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    chain.push_back(new BlockPow(chain.size(), prevHash, newHash, data, nonce, difficulty, 
                                 hashMode, rule, steps));
    
    std::cout << "Block #" << chain.size() - 1 << " mined in " << duration << " ms "
              << "(" << hashModeToString(hashMode) << ", " << nonce << " iterations)" << std::endl;
}

bool BlockchainPow::isChainValid() const {
    for (size_t i = 1; i < chain.size(); i++) {
        BlockPow* currentBlock = chain[i];
        
        //verify hash with the block's own hash mode
        if (!ProofOfWork::verifyBlock(currentBlock->getData(), 
                                     currentBlock->getPreviousHash(), 
                                     currentBlock->getHash(), 
                                     currentBlock->getDifficulty(), 
                                     currentBlock->getNonce(),
                                     currentBlock->getHashMode(),
                                     currentBlock->getRule(),
                                     currentBlock->getSteps())) {
            return false;
        }
        
        //verify chain linkage
        if (currentBlock->getPreviousHash() != chain[i-1]->getHash()) {
            return false;
        }
    }
    return true;
}

void BlockchainPow::displayChain() const {
    for (const auto* block : chain) {
        block->display();
    }
}

void BlockchainPow::setDifficulty(int diff) {
    difficulty = diff;
}

void BlockchainPow::setHashMode(HashMode mode, uint32_t r, size_t s) {
    hashMode = mode;
    rule = r;
    steps = s;
}

std::string BlockchainPow::getLatestHash() const {
    return chain.empty() ? "0" : chain.back()->getHash();
}

HashMode BlockchainPow::getHashMode() const {
    return hashMode;
}

uint32_t BlockchainPow::getRule() const {
    return rule;
}

size_t BlockchainPow::getSteps() const {
    return steps;
}

const std::vector<BlockPow*>& BlockchainPow::getChain() const {
    return chain;
}