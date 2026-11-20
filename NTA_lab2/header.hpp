#pragma once
#include <cstdint>
#include <map>

uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod);
uint64_t pow_mod(uint64_t a, uint64_t power, uint64_t mod);
bool is_prime(uint64_t n);
std::map<uint64_t, int> factorize(uint64_t n);
int64_t brute_force(uint64_t a, uint64_t b, uint64_t p, int timeoutSeconds = 300);
bool pohlig_hellman(uint64_t a, uint64_t b, uint64_t p, uint64_t& answer);
