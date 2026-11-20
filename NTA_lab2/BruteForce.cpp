#include "header.hpp"
#include <chrono>

using namespace std;

int64_t brute_force(uint64_t a, uint64_t b, uint64_t p, int timeoutSeconds) {
    auto start = chrono::steady_clock::now();
    uint64_t current = 1;
    a %= p;
    b %= p;
    for (uint64_t x = 0; x < p - 1; ++x) {
        if (current == b) return static_cast<int64_t>(x);
        current = mul_mod(current, a, p);
        if ((x & 1023) == 0 && chrono::steady_clock::now() - start >= chrono::seconds(timeoutSeconds))
            return -1;
    }
    return -1;
}
