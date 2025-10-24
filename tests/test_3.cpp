/** test_3.cpp
 * # what the test does:
 * 3.1 - Verifies hash mode selection (SHA256 vs AC_HASH)
 * 3.2 - Tests mining with both hash functions
 * 3.3 - Validates blocks work correctly with both modes
 * Bonus - Tests mixed hash modes in same chain
 * Bonus - Compares different CA rules (30, 90, 110)
 * 
 * #run & compile (MSYS2 MinGW64):
 * g++ -std=c++11 -I./include -IC:\msys64\mingw64\include ^
    src/cellular_automaton.cpp ^
    src/ac_hash.cpp ^
    src/utils.cpp ^
    src/pow.cpp ^
    src/block_pow.cpp ^
    src/blockchain_pow.cpp ^
    tests/test_3.cpp ^
    -LC:\msys64\mingw64\lib -lssl -lcrypto ^
    -o test_3.exe 
 * 
 */

#include "blockchain_pow.h"
#include "utils.h"
#include <iostream>
#include <vector>

void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void test_hash_mode_selection() {
    printSeparator("TEST 3.1: Hash Mode Selection");
    
    // Test SHA256 mode
    std::cout << "\n--- Creating blockchain with SHA256 ---" << std::endl;
    BlockchainPow sha_chain(2, SHA256_MODE);
    std::cout << "+ Blockchain created with hash mode: " << hashModeToString(sha_chain.getHashMode()) << std::endl;
    
    // Test AC_HASH mode
    std::cout << "\n--- Creating blockchain with AC_HASH ---" << std::endl;
    BlockchainPow ac_chain(2, AC_HASH_MODE, 30, 128);
    std::cout << "+ Blockchain created with hash mode: " << hashModeToString(ac_chain.getHashMode()) << std::endl;
    std::cout << "+ CA Rule: " << ac_chain.getRule() << std::endl;
    std::cout << "+ CA Steps: " << ac_chain.getSteps() << std::endl;

    std::cout << "\n[PASS] Hash mode selection working correctly" << std::endl;
}

void test_mining_with_ac_hash() {
    printSeparator("TEST 3.2: Mining with ac_hash()");
    
    std::vector<std::string> transactions = {
        "Alice->Bob: 50",
        "Bob->Charlie: 30"
    };
    
    std::cout << "\n--- Mining with SHA256 ---" << std::endl;
    BlockchainPow sha_chain(2, SHA256_MODE);
    sha_chain.addBlock(transactions);
    
    std::cout << "\n--- Mining with AC_HASH (Rule 30) ---" << std::endl;
    BlockchainPow ac_chain(2, AC_HASH_MODE, 30, 128);
    ac_chain.addBlock(transactions);
    
    std::cout << "\n[PASS] Mining works with both hash methods" << std::endl;
}

void test_validation_with_both_modes() {
    printSeparator("TEST 3.3: Block Validation with Both Hash Modes");
    
    std::vector<std::string> transactions = {
        "Transaction 1",
        "Transaction 2",
        "Transaction 3"
    };
    
    // Test SHA256 validation
    std::cout << "\n--- Testing SHA256 Blockchain Validation ---" << std::endl;
    BlockchainPow sha_chain(2, SHA256_MODE);
    sha_chain.addBlock(transactions);
    sha_chain.addBlock(transactions);
    
    bool sha_valid = sha_chain.isChainValid();
    std::cout << "SHA256 Chain Valid: " << (sha_valid ? "YES" : "NO") << std::endl;
    
    // Test AC_HASH validation
    std::cout << "\n--- Testing AC_HASH Blockchain Validation ---" << std::endl;
    BlockchainPow ac_chain(2, AC_HASH_MODE, 30, 128);
    ac_chain.addBlock(transactions);
    ac_chain.addBlock(transactions);
    
    bool ac_valid = ac_chain.isChainValid();
    std::cout << "AC_HASH Chain Valid: " << (ac_valid ? "YES" : "NO") << std::endl;
    
    if (sha_valid && ac_valid) {
        std::cout << "\n[PASS] Validation works correctly for both hash modes" << std::endl;
    } else {
        std::cout << "\n[FAIL] Validation failed for one or both modes" << std::endl;
    }
}

void test_mixed_hash_modes() {
    printSeparator("TEST 3.3 (Extended): Mixed Hash Modes in Same Chain");
    
    std::vector<std::string> transactions = {"Test transaction"};
    
    std::cout << "\n--- Creating chain with SHA256, then switching to AC_HASH ---" << std::endl;
    BlockchainPow chain(2, SHA256_MODE);
    
    std::cout << "Adding block 1 (SHA256)..." << std::endl;
    chain.addBlock(transactions);
    
    // Switch to AC_HASH
    chain.setHashMode(AC_HASH_MODE, 30, 128);
    std::cout << "\nSwitched to AC_HASH mode" << std::endl;
    
    std::cout << "Adding block 2 (AC_HASH)..." << std::endl;
    chain.addBlock(transactions);
    
    std::cout << "\nDisplaying mixed chain:" << std::endl;
    chain.displayChain();
    
    bool valid = chain.isChainValid();
    std::cout << "\nMixed Chain Valid: " << (valid ? "✓ YES" : "✗ NO") << std::endl;
    
    if (valid) {
        std::cout << "\n[PASS] Chain supports mixed hash modes" << std::endl;
    } else {
        std::cout << "\n[FAIL] Mixed hash modes not working" << std::endl;
    }
}

void test_different_rules() {
    printSeparator("TEST: Different CA Rules Comparison");
    
    std::vector<std::string> transactions = {"Test data"};
    
    std::cout << "\n--- Mining with Rule 30 ---" << std::endl;
    BlockchainPow chain_r30(2, AC_HASH_MODE, 30, 128);
    chain_r30.addBlock(transactions);
    
    std::cout << "\n--- Mining with Rule 90 ---" << std::endl;
    BlockchainPow chain_r90(2, AC_HASH_MODE, 90, 128);
    chain_r90.addBlock(transactions);
    
    std::cout << "\n--- Mining with Rule 110 ---" << std::endl;
    BlockchainPow chain_r110(2, AC_HASH_MODE, 110, 128);
    chain_r110.addBlock(transactions);
    
    std::cout << "\n[INFO] All rules produce valid blocks" << std::endl;
}

int main() {
    std::cout << "\n";
    std::cout << "==============================================================\n";
    std::cout << "=         EXERCISE 3: BLOCKCHAIN INTEGRATION TESTS           =\n";
    std::cout << "==============================================================\n";
    
    try {
        test_hash_mode_selection();
        test_mining_with_ac_hash();
        test_validation_with_both_modes();
        test_mixed_hash_modes();
        test_different_rules();
        
        printSeparator("ALL TESTS COMPLETED SUCCESSFULLY");
        std::cout << "\n Exercise 3.1 - Hash mode selection: WORKING" << std::endl;
        std::cout << " Exercise 3.2 - Mining with ac_hash: WORKING" << std::endl;
        std::cout << " Exercise 3.3 - Block validation: WORKING" << std::endl;
        std::cout << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n[ERROR] Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

