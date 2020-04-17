#ifndef SPM_GAMEOFLIFE_H
#define SPM_GAMEOFLIFE_H

#include <vector>
#include <random>
#include <omp.h>

#include "utils/utimer.hpp"


class GameOfLife{
public:
    explicit GameOfLife(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3);

    void printgrid(std::string filename = "");

    void reset(uint32_t seed_new = 1234);

    //an individual in a cell with 2 or  3 alive neighbours stays alive
    //an empty cell with exactly 3 alive neighbours becomes populated by a new individual
    //an alive cell with less than 2 alive neighbours dies (becomes empty)
    //an alive cell with more than 3 alive neighbours dies (become empty)

    virtual void play(uint16_t numIter, bool printGrid = false, uint16_t numw = 1) = 0;

protected:
    uint32_t n;
    uint32_t m;
    uint32_t seed;
    uint32_t density;
    uint16_t nw = 1;
    std::vector<std::vector<bool> > grid;
    std::vector<std::vector<bool> > grid_tmp;
};

GameOfLife::GameOfLife(uint32_t n, uint32_t m, uint32_t seed, float den) : n(n), m(m), seed(seed),
                                                                                 density(den * 100), grid(n),
                                                                                 grid_tmp(n) {

    for (int i = 0; i < n; ++i) {
        grid[i] = *new std::vector<bool>(m, false);
        grid_tmp[i] = *new std::vector<bool>(m, false);
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 99);
    for (int i = 1; i < n - 1; ++i) {
        for (int j = 1; j < m - 1; ++j) {
            if (dis(gen) < density) {
                grid[i][j] = true;
            }
        }
    }

}

void GameOfLife::reset(uint32_t seed_new) {
    seed = seed_new;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 99);
    for (int i = 1; i < n - 1; ++i) {
        for (int j = 1; j < m - 1; ++j) {
            grid_tmp[i][j] = false;
            if (dis(gen) < density) {
                grid[i][j] = true;
            } else {
                grid[i][j] = false;
            }
        }
    }
}

void GameOfLife::printgrid(const std::string filename) {
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    /*if (filename != "") {

        std::ofstream out(filename);
        out.open(filename);
        std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    }*/
    for (int i = 0; i < m; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j])
                std::cout << "0";
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < m; ++i) {
        std::cout << "-";
    }
    std::cout.rdbuf(coutbuf);
    std::cout << std::endl;
}

#endif //SPM_GAMEOFLIFE_H
