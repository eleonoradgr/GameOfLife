#ifndef SPM_GAMEOFLIFEOMPCHUNK_H
#define SPM_GAMEOFLIFEOMPCHUNK_H

#include <vector>
#include <random>
#include <omp.h>

#include "utils/utimer.hpp"
#include "GameOfLife.hpp"

class GameOfLifeOMPchunk : public GameOfLife {
public:
    explicit GameOfLifeOMPchunk(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3) : GameOfLife{n,
                                                                                                                      m,
                                                                                                                      seed,
                                                                                                                      den} {};

    void play(uint16_t numIter, bool printGrid = false, uint16_t numw = 1);

};

//similar to GameOfLifeOMP but I have fixed the chunk size and the schedule is static

void GameOfLifeOMPchunk::play(uint16_t numIter, bool printGrid, uint16_t numw) {

    nw = numw;
    uint64_t rep{n * m};
    uint64_t chunk{rep / nw};

    for (uint16_t iter = 0; iter < numIter; ++iter) {
#pragma omp parallel for num_threads(nw) schedule(static, chunk)
        for (int r = 0; r < rep; ++r) {
            int i{static_cast<int>(r / m)};
            int j{static_cast<int>(r % m)};
            if (i > 0 && j > 0 && i < m - 1 && j < n - 1) {
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

#endif //SPM_GAMEOFLIFEOMPCHUNK_H
