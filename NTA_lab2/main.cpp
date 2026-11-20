#include "header.hpp"
#include <chrono>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    uint64_t a, b, p;
    try {
        if (argc == 4) {
            a = stoull(argv[1]);
            b = stoull(argv[2]);
            p = stoull(argv[3]);
        } else {
            cout << "Solving a^x = b (mod p)\n";
            cout << "a: "; cin >> a;
            cout << "b: "; cin >> b;
            cout << "p: "; cin >> p;
            if (!cin) return 1;
        }
        if (p < 3 || p > INT64_MAX || !is_prime(p)) {
            cerr << "p must be an odd prime below 2^63\n";
            return 1;
        }
        auto start = chrono::steady_clock::now();
        uint64_t x = 0;
        bool ok = pohlig_hellman(a, b, p, x);
        auto finish = chrono::steady_clock::now();
        double sphMs = chrono::duration<double, milli>(finish - start).count();
        if (ok) cout << "SPH: " << x << " | " << sphMs << " ms\n";
        else cout << "SPH: no solution or a is not a generator\n";

        start = chrono::steady_clock::now();
        int64_t brute = brute_force(a, b, p, 5);
        finish = chrono::steady_clock::now();
        double bruteMs = chrono::duration<double, milli>(finish - start).count();
        if (brute >= 0) cout << "Brute force: " << brute << " | " << bruteMs << " ms\n";
        else cout << "Brute force: not found within 5 seconds\n";
        if (ok && brute >= 0 && x != static_cast<uint64_t>(brute)) {
            cerr << "Results do not match\n";
            return 2;
        }
        return ok ? 0 : 2;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
