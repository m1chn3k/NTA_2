#include "header.hpp"
#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

using namespace std;

static uint64_t add_mod(uint64_t a, uint64_t b, uint64_t mod) {
    a %= mod;
    b %= mod;
    return a >= mod - b ? a - (mod - b) : a + b;
}

uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t result = 0;
    a %= mod;
    while (b) {
        if (b & 1) result = add_mod(result, a, mod);
        b >>= 1;
        if (b) a = add_mod(a, a, mod);
    }
    return result;
}

uint64_t pow_mod(uint64_t a, uint64_t power, uint64_t mod) {
    uint64_t result = 1 % mod;
    a %= mod;
    while (power) {
        if (power & 1) result = mul_mod(result, a, mod);
        power >>= 1;
        if (power) a = mul_mod(a, a, mod);
    }
    return result;
}

bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while (!(d & 1)) { d >>= 1; ++s; }
    for (uint64_t a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
        if (a % n == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool passed = false;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { passed = true; break; }
        }
        if (!passed) return false;
    }
    return true;
}

static uint64_t rho(uint64_t n) {
    if (!(n & 1)) return 2;
    static mt19937_64 rng(random_device{}());
    for (int attempt = 0; attempt < 30; ++attempt) {
        uint64_t x = 2 + rng() % (n - 3), y = x;
        uint64_t c = 1 + rng() % (n - 1);
        auto step = [&](uint64_t v) { return add_mod(mul_mod(v, v, n), c, n); };
        for (int i = 0; i < 300000; ++i) {
            x = step(x);
            y = step(step(y));
            uint64_t delta = x > y ? x - y : y - x;
            uint64_t d = gcd(delta, n);
            if (d == 1) continue;
            if (d != n) return d;
            break;
        }
    }
    return 0;
}

static void split(uint64_t n, map<uint64_t, int>& result) {
    if (n == 1) return;
    if (is_prime(n)) { ++result[n]; return; }
    for (uint64_t p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL}) {
        if (n % p == 0) { split(p, result); split(n / p, result); return; }
    }
    uint64_t d = rho(n);
    if (!d) throw runtime_error("Could not factor p - 1");
    split(d, result);
    split(n / d, result);
}

map<uint64_t, int> factorize(uint64_t n) {
    map<uint64_t, int> result;
    split(n, result);
    return result;
}
