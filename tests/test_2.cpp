/**
 * g++ -I./include tests/test_2.cpp src/cellular_automaton.cpp src/ac_hash.cpp -o ./build/test_2.exe ; ./build/test_2.exe
 */

#include "ac_hash.h"
#include <iostream>
#include <iomanip>


//helper function
void print_test_header(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void test_question_1(){
    print_test_header("Test 2.1 : AC Haching function");
    std::cout << "The main function `ac_hash()` lives in ac_hash.cpp." << std::endl;
    std::cout << "How the hashing algorithms works :" << std::endl;
    std::cout << "1. The input is converted to a binary string." << std::endl;
    std::cout << "2. The binary string is hashed." << std::endl;
    std::cout << "3. The hashed string is converted back to a binary string." << std::endl;
    std::cout << "4. The binary string is converted to a hexadecimal string." << std::endl;
    std::cout << "5. The hexadecimal string is returned." << std::endl;
    std::cout << std::endl;
}

void test_different_inputs() {
    print_test_header("Test 2.4: Different Inputs Produce Different Hashes");
    
    std::string input1 = "Hello, World!";
    std::string input2 = "Hello, World?";
    std::string input3 = "Blockchain";
    
    uint32_t rule = 30;
    size_t steps = 128;
    
    std::string hash1 = ac_hash(input1, rule, steps);
    std::string hash2 = ac_hash(input2, rule, steps);
    std::string hash3 = ac_hash(input3, rule, steps);
    
    std::cout << "Input 1: \"" << input1 << "\"" << std::endl;
    std::cout << "Hash 1:  " << hash1 << std::endl;
    std::cout << "\nInput 2: \"" << input2 << "\"" << std::endl;
    std::cout << "Hash 2:  " << hash2 << std::endl;
    std::cout << "\nInput 3: \"" << input3 << "\"" << std::endl;
    std::cout << "Hash 3:  " << hash3 << std::endl;
    
    // Verify they are different
    std::cout << "\nHash 1 != Hash 2: " << (hash1 != hash2 ? "PASS" : "FAIL") << std::endl;
    std::cout << "Hash 1 != Hash 3: " << (hash1 != hash3 ? "PASS" : "FAIL") << std::endl;
    std::cout << "Hash 2 != Hash 3: " << (hash2 != hash3 ? "PASS" : "FAIL") << std::endl;
    
    // Check hash length is exactly 64 hex characters (256 bits)
    std::cout << "\nHash length = 64 chars (256 bits): " 
              << (hash1.length() == 64 ? "PASS" : "FAIL") << std::endl;
}

void test_same_input_reproducible() {
    print_test_header("Test: Same Input Produces Same Hash (Reproducibility)");
    
    std::string input = "Test message";
    uint32_t rule = 30;
    size_t steps = 128;
    
    std::string hash1 = ac_hash(input, rule, steps);
    std::string hash2 = ac_hash(input, rule, steps);
    
    std::cout << "Input: \"" << input << "\"" << std::endl;
    std::cout << "Hash (run 1): " << hash1 << std::endl;
    std::cout << "Hash (run 2): " << hash2 << std::endl;
    std::cout << "\nHashes match: " << (hash1 == hash2 ? "PASS" : "FAIL") << std::endl;
}

void test_empty_input() {
    print_test_header("Test: Empty Input Handling");
    
    std::string empty = "";
    std::string space = " ";
    
    std::string hash_empty = ac_hash(empty, 30, 128);
    std::string hash_space = ac_hash(space, 30, 128);
    
    std::cout << "Empty string hash: " << hash_empty << std::endl;
    std::cout << "Space char hash:   " << hash_space << std::endl;
    std::cout << "\nDifferent hashes: " << (hash_empty != hash_space ? "PASS" : "FAIL") << std::endl;
}

void test_text_to_bits_conversion() {
    print_test_header("Test 2.2: Text to Bits Conversion");
    std::cout << "the string_to_bits function in ac_hash.cpp converts an input string into bits by iterating over each character in the string. For each character, it performs a bitwise right shift operation to shift the bits of the character to the right by a specific number of positions. It then performs a bitwise AND operation with 1 to keep only the least significant bit of the shifted value. The resulting bit is added to a vector called bits. This process is repeated for each character in the string, resulting in a vector of bits that represents the input string." << std::endl;
    
    std::cout<<"\n Testing: " << std::endl;
    
    std::string test = "AB";
    std::vector<int> bits = string_to_bits(test);
    
    std::cout << "Input: \"" << test << "\"" << std::endl;
    std::cout << "ASCII: A=65, B=66" << std::endl;
    std::cout << "Expected bits: 01000001 01000010" << std::endl;
    std::cout << "Actual bits:   ";
    
    for (size_t i = 0; i < bits.size(); i++) {
        std::cout << bits[i];
        if ((i + 1) % 8 == 0 && i + 1 < bits.size()) std::cout << " ";
    }
    std::cout << std::endl;
    
    // Verify
    bool correct = (bits.size() == 16 && 
                   bits[0] == 0 && bits[1] == 1 && bits[6] == 0 && bits[7] == 1 &&  // 'A' = 01000001
                   bits[8] == 0 && bits[9] == 1 && bits[14] == 1 && bits[15] == 0); // 'B' = 01000010
    
    std::cout << "Conversion correct: " << (correct ? "PASS" : "FAIL") << std::endl;
}

int main() {
    std::cout << "\n";
    std::cout << "==============================================================\n";
    std::cout << "=                     AC_HASH FUNCTION TESTS                 =\n";
    std::cout << "==============================================================\n";
    
    test_text_to_bits_conversion();
    std::cout << "Test 2.3. Check README.md file for explanation section ###2.3" << std::endl;
    test_different_inputs();
    test_same_input_reproducible();
    test_empty_input();
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "All tests completed!" << std::endl;
    std::cout << std::string(60, '=') << "\n" << std::endl;
    
    return 0;
}



