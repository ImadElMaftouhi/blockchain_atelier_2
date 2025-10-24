#include "utils.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <chrono>


std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hash);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string getCurrentTime() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t t = system_clock::to_time_t(now);
    std::tm bt;
#ifdef _WIN32
    localtime_s(&bt, &t);
#else
    localtime_r(&t, &bt);
#endif
    std::ostringstream oss;
    oss << std::put_time(&bt, "%a %b %d %H:%M:%S");
    oss << "." << std::setw(3) << std::setfill('0') << ms.count();
    return oss.str();
}