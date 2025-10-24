/**
 * g++ -std=c++11 -I./include -IC:\msys64\mingw64\include src/cellular_automaton.cpp src/ac_hash.cpp src/utils.cpp src/pow.cpp src/block_pow.cpp src/blockchain_pow.cpp tests/test_4_benchmark.cpp -LC:\msys64\mingw64\lib -lssl -lcrypto -o test_4_benchmark.exe
 * 
 */

#include "blockchain_pow.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>

struct BenchmarkResult {
    std::string hashMethod;
    int difficulty;
    long long totalTime_ms;
    double avgTime_ms;
    long long totalIterations;
    double avgIterations;
    int blocksCount;
};

void printTableHeader() {
    std::cout << "\n" << std::string(90, '=') << std::endl;
    std::cout << std::left 
              << std::setw(15) << "Hash Method"
              << std::setw(12) << "Difficulty"
              << std::setw(15) << "Total Time(ms)"
              << std::setw(15) << "Avg Time(ms)"
              << std::setw(18) << "Total Iterations"
              << std::setw(15) << "Avg Iterations"
              << std::endl;
    std::cout << std::string(90, '-') << std::endl;
}

void printTableRow(const BenchmarkResult& result) {
    std::cout << std::left 
              << std::setw(15) << result.hashMethod
              << std::setw(12) << result.difficulty
              << std::setw(15) << result.totalTime_ms
              << std::setw(15) << std::fixed << std::setprecision(2) << result.avgTime_ms
              << std::setw(18) << result.totalIterations
              << std::setw(15) << std::fixed << std::setprecision(2) << result.avgIterations
              << std::endl;
}

BenchmarkResult benchmarkHashMethod(HashMode mode, int difficulty, int numBlocks, 
                                     uint32_t rule = 30, size_t steps = 128) {
    BenchmarkResult result;
    result.hashMethod = hashModeToString(mode);
    result.difficulty = difficulty;
    result.blocksCount = numBlocks;
    result.totalTime_ms = 0;
    result.totalIterations = 0;
    
    std::vector<std::string> transactions = {
        "Alice->Bob: 50",
        "Bob->Charlie: 30",
        "Charlie->Dave: 20"
    };
    
    BlockchainPow chain(difficulty, mode, rule, steps);
    
    //mine
    for (int i = 0; i < numBlocks; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        
        // Get state before adding block
        int initialChainSize = chain.getChain().size();
        
        chain.addBlock(transactions);
        
        auto end = std::chrono::high_resolution_clock::now();
        long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        // Get the nonce from the last added block
        const BlockPow* lastBlock = chain.getChain().back();
        int nonce = lastBlock->getNonce();
        
        result.totalTime_ms += duration;
        result.totalIterations += nonce;
    }
    
    result.avgTime_ms = static_cast<double>(result.totalTime_ms) / numBlocks;
    result.avgIterations = static_cast<double>(result.totalIterations) / numBlocks;
    
    return result;
}

void test_MiningTime(int numBlocks = 10, int difficulty = 2) {
    std::cout << "\n==============================================================";
    std::cout << "\n=            Mining Time and Iterations Comparison           =";
    std::cout << "\n==============================================================";
    
    std::cout << "\nMining " << numBlocks << " blocks with difficulty " << difficulty << "...\n";
    
    //SHA256
    std::cout << "\n--- Benchmarking SHA256 ---" << std::endl;
    BenchmarkResult sha_result = benchmarkHashMethod(SHA256_MODE, difficulty, numBlocks);
    
    //AC_HASH
    std::cout << "\n--- Benchmarking AC_HASH (Rule 30, 128 steps) ---" << std::endl;
    BenchmarkResult ac_result = benchmarkHashMethod(AC_HASH_MODE, difficulty, numBlocks, 30, 128);
    
    printTableHeader();
    printTableRow(sha_result);
    printTableRow(ac_result);
    std::cout << std::string(90, '=') << std::endl;
    
    // Analysis
    std::cout << "\n--- Analysis ---" << std::endl;
    double timeRatio = static_cast<double>(ac_result.totalTime_ms) / sha_result.totalTime_ms;
    std::cout << "AC_HASH is " << std::fixed << std::setprecision(2) << timeRatio << "x " 
              << (timeRatio > 1 ? "SLOWER" : "FASTER") << " than SHA256" << std::endl;
    
    double iterRatio = ac_result.avgIterations / sha_result.avgIterations;
    std::cout << "AC_HASH requires " << std::fixed << std::setprecision(2) << iterRatio << "x " 
              << (iterRatio > 1 ? "MORE" : "FEWER") << " iterations on average" << std::endl;
}

void comparison_table() {
    std::cout << "\n\n=========================================================\n";
    std::cout << "=                   Complete Comparison Table               =\n";
    std::cout << "=============================================================\n";
    
    int numBlocks = 10;
    std::vector<int> difficulties = {2, 3};
    std::vector<BenchmarkResult> allResults;
    
    for (int diff : difficulties) {
        std::cout << "\n=== Testing with Difficulty " << diff << " ===" << std::endl;
        
        std::cout << "Mining with SHA256..." << std::endl;
        BenchmarkResult sha_result = benchmarkHashMethod(SHA256_MODE, diff, numBlocks);
        allResults.push_back(sha_result);
        
        std::cout << "Mining with AC_HASH..." << std::endl;
        BenchmarkResult ac_result = benchmarkHashMethod(AC_HASH_MODE, diff, numBlocks, 30, 128);
        allResults.push_back(ac_result);
    }
    
    std::cout << "\n\n";
    std::cout << "====================================================\n";
    std::cout << "=              FINAL COMPARISON RESULTS            =\n";
    std::cout << "====================================================\n";
    
    printTableHeader();
    for (const auto& result : allResults) {
        printTableRow(result);
    }
    std::cout << std::string(90, '=') << std::endl;
    
    // Summary
    std::cout << "\n--- Summary ---" << std::endl;
    std::cout << "SHA256: Fast per hash, but requires many iterations to find valid hash" << std::endl;
    std::cout << "AC_HASH: Slower per hash due to CA evolution, but similar iteration count" << std::endl;
    std::cout << "For blockchain: SHA256 is generally faster due to hardware optimization" << std::endl;
    std::cout << "AC_HASH provides alternative security model based on CA complexity" << std::endl;
}

void test_different_difficulties() {
    std::cout << "\n\n==========================================================\n";
    std::cout << "=         Additional Test: Different Difficulty Levels       =\n";
    std::cout << "==============================================================\n";
    
    std::vector<int> difficulties = {1, 2, 3, 4};
    int numBlocks = 5;
    
    printTableHeader();
    
    for (int diff : difficulties) {
        std::cout << "\nTesting difficulty " << diff << "..." << std::endl;
        
        BenchmarkResult sha_result = benchmarkHashMethod(SHA256_MODE, diff, numBlocks);
        printTableRow(sha_result);
        
        BenchmarkResult ac_result = benchmarkHashMethod(AC_HASH_MODE, diff, numBlocks, 30, 128);
        printTableRow(ac_result);
        
        std::cout << std::string(90, '-') << std::endl;
    }
}

int main() {
    std::cout << "\n";
    std::cout << "==============================================================\n";
    std::cout << "=            EXERCISE 4: PERFORMANCE BENCHMARKING            =\n";
    std::cout << "==============================================================\n";
    
    try {
        //question 4.1 and 4.2
        test_MiningTime(10, 2);
        
        //question 4.3
        comparison_table();
        
        //Test different difficulties
        test_different_difficulties();
        
        std::cout << "\n\n==========================================================\n";
        std::cout << "=              BENCHMARK COMPLETED SUCCESSFULLY              =\n";
        std::cout << "==============================================================\n\n";
        
    } catch (const std::exception& e) {
        std::cerr << "\n[ERROR] Benchmark failed: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}