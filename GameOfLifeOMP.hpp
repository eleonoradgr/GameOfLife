#ifndef SPM_GAMEOFLIFEOMP_H
#define SPM_GAMEOFLIFEOMP_H

#include <vector>
#include <random>
#include <omp.h>

#include "utils/utimer.hpp"
#include "GameOfLife.hpp"

class GameOfLifeOMP : public GameOfLife {
public:
    explicit GameOfLifeOMP(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3) : GameOfLife{n, m,
                                                                                                                 seed,
                                                                                                                 den} {};


    void play(uint16_t numIter, bool printGrid = false, uint16_t numw = 1);

};

//provides the sequential implementation extended with "#pragma omp parallel for num_threads(nw) schedule(auto)",
// so all is managed by OpenMP

void GameOfLifeOMP::play(uint16_t numIter, bool printGrid, uint16_t numw) {

    nw = numw;
    uint32_t delta{(n - 2) / nw};

    auto t3 = omp_get_wtime();
    for (uint16_t iter = 0; iter < numIter; ++iter) {
#pragma omp parallel for num_threads(nw) schedule(auto)
        for (int i = 1; i < n - 1; ++i) {
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

}


#endif //SPM_GAMEOFLIFEOMP_H
