#include "header.hpp"
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <vector>

using namespace std;

static uint64_t inverse(uint64_t a, uint64_t p) {
    return pow_mod(a, p - 2, p);
}

static bool digit_log(uint64_t base, uint64_t target, uint64_t order, uint64_t p, uint64_t& digit) {
    if (order <= 100000) {
        uint64_t cur = 1;
        for (uint64_t j = 0; j < order; ++j) {
            if (cur == target) { digit = j; return true; }
            cur = mul_mod(cur, base, p);
        }
        return false;
    }
    uint64_t m = static_cast<uint64_t>(sqrtl(static_cast<long double>(order))) + 1;
    if (m > 1500000) throw runtime_error("Prime factor too large for the current BSGS limit");
    unordered_map<uint64_t, uint64_t> table;
    table.reserve(static_cast<size_t>(m * 1.4));
    uint64_t cur = 1;
    for (uint64_t j = 0; j < m; ++j) {
        table.emplace(cur, j);
        cur = mul_mod(cur, base, p);
    }
    uint64_t step = inverse(pow_mod(base, m, p), p);
    cur = target;
    for (uint64_t i = 0; i <= m; ++i) {
        auto it = table.find(cur);
        if (it != table.end() && i * m + it->second < order) {
            digit = i * m + it->second;
            return true;
        }
        cur = mul_mod(cur, step, p);
    }
    return false;
}

static bool solve_prime_power(uint64_t a, uint64_t b, uint64_t p, uint64_t n,
                              uint64_t q, int exponent, uint64_t& residue, uint64_t& modulus) {
    uint64_t base = pow_mod(a, n / q, p);
    uint64_t qpower = 1;
    uint64_t x = 0;
    for (int i = 0; i < exponent; ++i) {
        if (qpower > n / q) return false;
        uint64_t nextPower = qpower * q;
        uint64_t correction = pow_mod(inverse(a, p), x, p);
        uint64_t value = mul_mod(b, correction, p);
        value = pow_mod(value, n / nextPower, p);
        uint64_t digit = 0;
        if (!digit_log(base, value, q, p, digit)) return false;
        x += digit * qpower;
        qpower = nextPower;
    }
    residue = x;
    modulus = qpower;
    return true;
}

bool pohlig_hellman(uint64_t a, uint64_t b, uint64_t p, uint64_t& answer) {
    if (p < 3 || !is_prime(p)) return false;
    a %= p;
    b %= p;
    if (!a || !b) return false;
    uint64_t n = p - 1;
    auto factors = factorize(n);
    for (auto [q, exp] : factors) {
        if (pow_mod(a, n / q, p) == 1) return false;
    }

    uint64_t x = 0, product = 1;
    for (auto [q, exp] : factors) {
        uint64_t residue = 0, modulus = 0;
        if (!solve_prime_power(a, b, p, n, q, exp, residue, modulus)) return false;
        uint64_t remainder = residue >= x % modulus
            ? residue - x % modulus : modulus - (x % modulus - residue);
        uint64_t base = product % modulus;
        uint64_t phi = modulus / q * (q - 1);
        uint64_t modularInverse = pow_mod(base, phi - 1, modulus);
        uint64_t multiplier = mul_mod(remainder, modularInverse, modulus);
        uint64_t term = product * multiplier;
        x += term;
        product *= modulus;
    }
    answer = x % n;
    return pow_mod(a, answer, p) == b;
}
