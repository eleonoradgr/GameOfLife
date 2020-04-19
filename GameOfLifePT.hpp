#ifndef SPM_GAMEOFLIFEPT_HPP
#define SPM_GAMEOFLIFEPT_HPP

#include <vector>
#include <bitset>
#include <random>
#include <thread>
#include <fstream>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "utils/utimer.hpp"

typedef struct {
    int row;
    int start;
    int end;
} RANGE2;


class GameOfLifePT : public GameOfLife {
public:
    explicit GameOfLifePT(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3) : GameOfLife{n,
                                                                                                                m,
                                                                                                                seed,
                                                                                                                den} {};

    void play(uint16_t numIter, bool printGrid = false, uint16_t numw = 1);

};

// a pool of thread is created according to the input parameter, and at each iteration,
// each thread computes the set of rows or part of it (if rows are less then workers), according to its concern.
// Two condition variables are used to coordinate operations of swap and passage to the next iteration

void GameOfLifePT::play(uint16_t numIter, bool printGrid, uint16_t numw) {

    nw = numw;
    std::vector<RANGE2> ranges(nw);
    uint32_t delta{(n - 2) / nw};
    std::vector<std::thread> tids;

    //compute the value of a single cell of position i j
    auto compute_cell = [&](int i, int j) {
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
    };

    std::atomic<int> iter = 0;
    std::atomic<int> finished = 0;
    std::mutex mutex;
    std::condition_variable workers;
    std::condition_variable emitter;
    std::function<void(RANGE2)> compute_rows;

    if (delta >= 1) {
        // if workers are less then/ equal to  the number of rows, a set of delta rows is assigned to each of them

        for (int i = 0; i < nw; i++) {
            ranges[i].start = i * delta + 1;
            ranges[i].end = (i != (nw - 1) ? (i + 1) * delta + 1 : n - 1);
        }

        compute_rows = [&](RANGE2 range) {
            while (iter < numIter) {
                for (int i = range.start; i < range.end; ++i) {
                    for (int j = 1; j < m - 1; ++j) {
                        compute_cell(i, j);
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
    } else {
        // if workers are more thenthe number of rows, a set of new_delta+1 items is assigned to each of them

        uint32_t row = 1;
        uint64_t first = 1;
        uint64_t new_delta{(n - 2) * (m - 2) / nw};

        for (int i = 0; i < nw; i++) {
            ranges[i].row = row;
            ranges[i].start = first;
            ranges[i].end = first + new_delta;

            if (i == (nw - 1) && ranges[i].end >= m - 1) {
                ranges[i].end = m - 2;
            }
            if (ranges[i].end >= m - 1) {
                ranges[i].end = ranges[i].end % (m - 2);
                row++;
            }
            first = ranges[i].end;

        }

        compute_rows = [&](RANGE2 range) {
            while (iter < numIter) {
                if (range.start < range.end) {
                    int32_t i = range.row;
                    for (int j = range.start; j < range.end; ++j) {
                        compute_cell(i, j);
                    }
                } else {
                    int i = range.row;
                    for (int j = range.start; j < m - 1; ++j) {
                        compute_cell(i, j);
                    }
                    i++;
                    for (int j = 1; j < range.end; ++j) {
                        compute_cell(i, j);
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
    }

    for (int i = 0; i < nw; i++) {
        tids.push_back(std::thread(compute_rows, ranges[i]));
    }

    while (iter < numIter) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            while (finished < nw) {
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


#endif //SPM_GAMEOFLIFEPT_HPP
