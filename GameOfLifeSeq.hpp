#ifndef SPM_GAMEOFLIFESEQ_H
#define SPM_GAMEOFLIFESEQ_H

#include <vector>
#include <bitset>
#include <random>
#include "utils/utimer.hpp"
#include "GameOfLife.hpp"


class GameOfLifeSeq : public GameOfLife {
public:

    explicit GameOfLifeSeq(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3) : GameOfLife{n, m, seed, den}{};

    //an individual in a cell with 2 or  3 alive neighbours stays alive
    //an empty cell with exactly 3 alive neighbours becomes populated by a new individual
    //an alive cell with less than 2 alive neighbours dies (becomes empty)
    //an alive cell with more than 3 alive neighbours dies (become empty)

    void play(uint16_t numIter, bool printGrid = false, uint8_t nw = 1);

};

void GameOfLifeSeq::play(uint16_t numIter, bool printGrid, uint8_t nw ) {

    utimer seq(std::to_string(numIter) + " iterations of sequential Game of Life");

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
