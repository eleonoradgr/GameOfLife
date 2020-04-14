#ifndef SPM_GAMEOFLIFEOMP_H
#define SPM_GAMEOFLIFEOMP_H

#include <vector>
#include <random>
#include <omp.h>

#include "utils/utimer.hpp"


class GameOfLifeOMP{
public:
        explicit GameOfLifeOMP(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3);

        void printgrid(std::string filename = "");

        void reset(uint32_t seed_new = 1234);

        //an individual in a cell with 2 or  3 alive neighbours stays alive
        //an empty cell with exactly 3 alive neighbours becomes populated by a new individual
        //an alive cell with less than 2 alive neighbours dies (becomes empty)
        //an alive cell with more than 3 alive neighbours dies (become empty)

        void play(uint16_t numIter, uint8_t numw, bool printGrid = false);

private:
        uint32_t n;
        uint32_t m;
        uint32_t seed;
        uint32_t density;
        uint16_t nw = 1;
        std::vector<std::vector<bool> > grid;
        std::vector<std::vector<bool> > grid_tmp;
};

GameOfLifeOMP::GameOfLifeOMP(uint32_t n, uint32_t m, uint32_t seed, float den) : n(n), m(m), seed(seed),
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

void GameOfLifeOMP::reset(uint32_t seed_new) {
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

void GameOfLifeOMP::printgrid(const std::string filename) {
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    if (filename != "") {

        std::ofstream out(filename);
        out.open(filename);
        std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    }
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

void GameOfLifeOMP::play(uint16_t numIter, uint8_t numw, bool printGrid) {

    utimer seq(std::to_string(numIter) + " iterations of parallel OMP Game of Life");
    nw = numw;
    uint32_t delta{(n - 2) / nw};

    auto t1 = omp_get_wtime();
    for (uint16_t iter = 0; iter < numIter; ++iter) {
#pragma omp parallel for num_threads(nw) schedule(static, delta)
        for (int i = 1; i < n-1; ++i) {
            for (int j = 1; j < m - 1; ++j) {
                int8_t sum = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1]
                             + grid[i][j - 1] + grid[i][j + 1]
                             + grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];
                if (sum == 2) {
                    grid_tmp[i][j] = grid[i][j];
                } else if (sum == 3) {
                    grid_tmp[i][j] = true;
                } else {
                    grid_tmp[i][j] = false;
                }
            }
        }
        grid.swap(grid_tmp);
        if (printGrid)
            printgrid();

    }
    auto par = omp_get_wtime() - t1;


  /*  reset();
    auto t3 = omp_get_wtime();
    for (uint16_t iter = 0; iter < numIter; ++iter) {
#pragma omp parallel for num_threads(nw) schedule(auto)
        for (int i = 1; i < n-1; ++i) {
            for (int j = 1; j < m - 1; ++j) {
                int8_t sum = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1]
                             + grid[i][j - 1] + grid[i][j + 1]
                             + grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];
                if (sum == 2) {
                    grid_tmp[i][j] = grid[i][j];
                } else if (sum == 3) {
                    grid_tmp[i][j] = true;
                } else {
                    grid_tmp[i][j] = false;
                }
            }
        }
        grid.swap(grid_tmp);
        if (printGrid)
            printgrid();

    }
    auto par3 = omp_get_wtime() - t3;

    std::cout << "OMP static  time is " << par << std::endl
         << "OMP auto    time is " << par3 << std::endl;*/

}


#endif //SPM_GAMEOFLIFEOMP_H
