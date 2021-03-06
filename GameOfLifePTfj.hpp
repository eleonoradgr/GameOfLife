#ifndef SPM_GAMEOFLIFEPT_H
#define SPM_GAMEOFLIFEPT_H

#include <vector>
#include <bitset>
#include <random>
#include <thread>
#include <fstream>
#include "utils/utimer.hpp"


typedef struct {
    int start;
    int end;
} RANGE;


class GameOfLifePTfj : public GameOfLife {
public:
    explicit GameOfLifePTfj(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3) : GameOfLife{n, m,
                                                                                                                  seed,
                                                                                                                  den} {};


    void play(uint16_t numIter, bool printGrid = false, uint16_t numw = 1);

};

//it's just an experiment in which all threads are created at each iteration

void GameOfLifePTfj::play(uint16_t numIter, bool printGrid, uint16_t numw) {

    nw = numw;
    std::vector<RANGE> ranges(nw);                     // vector to compute the ranges
    uint32_t delta{(n - 2) / nw};
    std::vector<std::thread> tids;

    for (int i = 0; i < nw; i++) {
        ranges[i].start = i * delta + 1;
        ranges[i].end = (i != (nw - 1) ? (i + 1) * delta + 1 : n - 1);
    }

    auto compute_rows = [&](RANGE range) {
        for (int i = range.start; i < range.end; ++i) {
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
    };

    for (uint16_t iter = 0; iter < numIter; ++iter) {

        for (int i = 0; i < nw; i++) {                     // assign chuncks to threads
            tids.push_back(std::thread(compute_rows, ranges[i]));
        }

        for (std::thread &t: tids) {                        // await thread termination
            t.join();
        }
        tids.clear();
        grid.swap(grid_tmp);
        if (printGrid)
            printgrid();
    }
}

#endif //SPM_GAMEOFLIFEPT_H
