#ifndef SPM_GAMEOFLIFESEQ_H
#define SPM_GAMEOFLIFESEQ_H

#include <vector>
#include <bitset>
#include <random>
#include "utils/utimer.hpp"
#include "GameOfLife.hpp"


class GameOfLifeSeq : public GameOfLife {
public:

    explicit GameOfLifeSeq(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3) : GameOfLife{n, m,
                                                                                                                 seed,
                                                                                                                 den} {};


    void play(uint16_t numIter, bool printGrid = false, uint16_t nw = 1);

};

// provides a trivial sequential implementation which scans all the cells of the grid through two nested for.
// The computed value is stored in a new grid and then grids are swapped.

void GameOfLifeSeq::play(uint16_t numIter, bool printGrid, uint16_t nw) {

    for (uint16_t iter = 0; iter < numIter; ++iter) {
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


#endif //SPM_GAMEOFLIFESEQ_H
