// A library of functions to be wrapped into a python module.

#include "../include/cython_bindings.hpp"
#include "../include/OctantDonutSieve.hpp"
#include "../include/SectorSieve.hpp"
#include "../include/BlockSieve.hpp"
#include <iostream>
#include <cmath>


// Getting Gaussian primes upto a given norm. Python will hold these as a list
// of tuples. Each tuples holds the real and imaginary parts of each prime.
vector<pair<uint32_t, uint32_t>> gPrimesToNorm(uint64_t x) {
    if (x >= pow(2, 30)) {
        cerr << "Large norm bound makes copying C++ containers to python\n"
             << "containers too slow. Call gPrimesAsArray() instead.\n" << endl;
        exit(1);
    }
    // Trigger verbose mode if passed argument is large.
    bool verbose = x >= (uint64_t)pow(10, 7);
    OctantDonutSieve s(x, verbose);
    s.run();
    vector<gint> gintP = s.getBigPrimes();
    vector<pair<uint32_t, uint32_t>> pairP;
    pairP.resize(gintP.size());
    transform(gintP.begin(), gintP.end(), pairP.begin(), [](gint g) { return g.asPair(); });  // lambda
    cout << "Copying sieve array to python list of tuples...." << endl;
    return pairP;
}

//  Getting Gaussian primes in sector upto a given norm.
vector<pair<uint32_t, uint32_t>> gPrimesInSector(uint64_t x, double alpha, double beta) {
    if (x >= pow(2, 30)) {
        cerr << "Large norm bound makes copying C++ containers to python\n"
             << "containers too slow. Call gPrimesAsArray() instead.\n" << endl;
        exit(1);
    }
    // Trigger verbose mode if passed argument is large.
    bool verbose = x >= (uint64_t)pow(10, 7);
    SectorSieve s(x, alpha, beta, verbose);
    s.run();
    vector<gint> gintP = s.getBigPrimes();
    vector<pair<uint32_t, uint32_t>> pairP;
    pairP.resize(gintP.size());
    transform(gintP.begin(), gintP.end(), pairP.begin(), [](gint g) { return g.asPair(); });  // lambda
    cout << "Copying sieve array to python list of tuples...." << endl;
    return pairP;
}

// Gaussian integers in block defined by intervals [x, x + dx) and [y, y + dy).
vector<pair<uint32_t, uint32_t>> gPrimesInBlock(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy) {
    if (dx * dy >= pow(2, 30)) {
        cerr << "Large norm bound makes copying C++ containers to python\n"
             << "containers too slow. Call gPrimesAsArray() instead.\n" << endl;
        exit(1);
    }
    // Trigger verbose mode if passed argument is large.
    bool verbose = dx * dy >= (uint64_t)pow(10, 7);
    BlockSieve s(x, y, dx, dy, verbose);
    s.run();
    vector<gint> gintP = s.getBigPrimes();
    vector<pair<uint32_t, uint32_t>> pairP;
    pairP.resize(gintP.size());
    transform(gintP.begin(), gintP.end(), pairP.begin(), [](gint g) { return g.asPair(); });  // lambda
    cout << "Copying sieve array to python list of tuples...." << endl;
    return pairP;
}

// Counting Gaussian primes and associates upto a given norm.
uint64_t gPrimesToNormCount(uint64_t x) {
    // Show display if passed argument is large.
    bool verbose = x >= (uint64_t)pow(10, 9);
    OctantDonutSieve s(x, verbose);
    s.run();
    return s.getCountBigPrimes();
}

// Counting Gaussian primes in sector upto a given norm.
uint64_t gPrimesInSectorCount(uint64_t x, double alpha, double beta) {
    // Show display if passed argument is large.
    bool verbose = x >= (uint64_t)pow(10, 9);
    SectorSieve s(x, alpha, beta, verbose);
    s.run();
    return s.getCountBigPrimes();
}

// Counting Gaussian primes in sector upto a given norm.
uint64_t gPrimesInBlockCount(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy) {
    // Show display if passed argument is large.
    bool verbose = dx * dy >= (uint64_t)pow(10, 9);
    BlockSieve s(x, y, dx, dy, verbose);
    s.run();
    return s.getCountBigPrimes();
}

// Getting Gaussian primes upto a given norm. Passing a pointer to an array in
// c++, so data is not explicitly copied between memory controlled by c++ and
// python.
pair<uint32_t *, uint64_t> gPrimesToNormAsArray(uint64_t x) {
    OctantDonutSieve s(x);
    s.run();
    vector<gint> gintP = s.getBigPrimes();
    // Creating a 1-dimensional array to hold big primes; this way we can avoid
    // an array of pointers which might be needed for 2d array.
    uint64_t size = gintP.size();
    auto *P = new uint32_t[2 * size];  // declaring the array
    for (uint64_t i = 0; i < size; i++) {
        P[2 * i] = gintP[i].a;
        P[2 * i + 1] = gintP[i].b;
    }
    cout << P << endl;
    return pair<uint32_t *, uint64_t> {P, 2 * size};
}

// Getting Gaussian primes in sector upto given norm. Return pointer to an array.
pair<uint32_t *, uint64_t> gPrimesInSectorAsArray(uint64_t x, double alpha, double beta) {
    SectorSieve s(x, alpha, beta);
    s.run();
    vector<gint> gintP = s.getBigPrimes();
    // Creating a 1-dimensional array to hold big primes; this way we can avoid
    // an array of pointers which might be needed for 2d array.
    uint64_t size = gintP.size();
    auto *P = new uint32_t[2 * size];  // declaring the array
    for (uint64_t i = 0; i < size; i++) {
        P[2 * i] = gintP[i].a;
        P[2 * i + 1] = gintP[i].b;
    }
    cout << P << endl;
    return pair<uint32_t *, uint64_t> {P, 2 * size};
}

// Getting Gaussian primes in block. Return pointer to an array.
pair<uint32_t *, uint64_t> gPrimesInBlockAsArray(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy) {
    BlockSieve s(x, y, dx, dy);
    s.run();
    vector<gint> gintP = s.getBigPrimes();
    // Creating a 1-dimensional array to hold big primes; this way we can avoid
    // an array of pointers which might be needed for 2d array.
    uint64_t size = gintP.size();
    auto *P = new uint32_t[2 * size];  // declaring the array
    for (uint64_t i = 0; i < size; i++) {
        P[2 * i] = gintP[i].a;
        P[2 * i + 1] = gintP[i].b;
    }
    cout << P << endl;
    return pair<uint32_t *, uint64_t> {P, 2 * size};
}