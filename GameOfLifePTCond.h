#ifndef SPM_GAMEOFLIFEPTCOND_H
#define SPM_GAMEOFLIFEPTCOND_H


#include <vector>
#include <bitset>
#include <random>
#include <thread>
#include <fstream>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "utils/utimer.hpp"



class GameOfLifePTCond : public GameOfLife{
public:
    explicit GameOfLifePTCond(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3): GameOfLife{n, m, seed, den}{};

    //an individual in a cell with 2 or  3 alive neighbours stays alive
    //an empty cell with exactly 3 alive neighbours becomes populated by a new individual
    //an alive cell with less than 2 alive neighbours dies (becomes empty)
    //an alive cell with more than 3 alive neighbours dies (become empty)

    void play(uint16_t numIter, bool printGrid = false, uint8_t numw = 1);

};


void GameOfLifePTCond::play(uint16_t numIter, bool printGrid, uint8_t numw) {

    utimer seq(std::to_string(numIter) + " iterations of parallel cond Game of Life");
    nw = numw;
    std::vector<RANGE> ranges(nw);                     // vector to compute the ranges
    uint32_t delta{(n - 2) / nw};
    std::vector<std::thread> tids;

    for (int i = 0; i < nw; i++) {                     // split the string into pieces
        ranges[i].start = i * delta + 1;
        ranges[i].end = (i != (nw - 1) ? (i + 1) * delta + 1 : n - 1);
    }

    std::atomic<int> iter = 0;
    std::atomic<int> finished = 0;
    std::mutex mutex;
    std::condition_variable workers;
    std::condition_variable emitter;


    auto compute_rows = [&](RANGE range) {
        while ( iter < numIter) {
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
            {
                std::unique_lock<std::mutex> lock(mutex);
                finished++;
                emitter.notify_one();
                workers.wait(lock);
            }

        }
    };

    for (int i = 0; i < nw; i++) {                     // assign chuncks to threads
        tids.push_back(std::thread(compute_rows, ranges[i]));
    }

    while( iter < numIter){
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(finished < nw){
                emitter.wait(lock);
            }
        }
        finished = 0;
        iter++;
        grid.swap(grid_tmp);
        if (printGrid)
            printgrid();
        workers.notify_all();

    }

    for (std::thread &t: tids) {                        // await thread termination
        t.join();
    }
    tids.clear();


}

#endif //SPM_GAMEOFLIFEPTCOND_H