#ifndef SPM_GAMEOFLIFEPTVEC_H
#define SPM_GAMEOFLIFEPTVEC_H

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



class GameOfLifePTvec  {
public:
    explicit GameOfLifePTvec(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3);

    void printgrid(const char *filename = "");

    void reset(uint32_t seed_new = 1234);

    void play(uint16_t numIter, bool printGrid = false, uint16_t numw = 1);

protected:
    uint32_t n;
    uint32_t m;
    uint32_t seed;
    uint32_t density;
    uint16_t nw = 1;
    std::vector<std::vector<uint16_t> > grid;
    std::vector<std::vector<uint16_t> > grid_tmp;

};

GameOfLifePTvec::GameOfLifePTvec(uint32_t n, uint32_t m, uint32_t seed, float den) : n(n), m(m), seed(seed),
                                                                                               density(den * 100),grid(n),
                                                                                               grid_tmp(n) {

    for (int i = 0; i < n; ++i) {
        grid[i] = *new std::vector<uint16_t>(m, 0);
        grid_tmp[i] = *new std::vector<uint16_t>(m, 0);
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 99);
    for (int i = 1; i < n - 1; ++i) {
        for (int j = 1; j < m - 1; ++j) {
            if (dis(gen) < density) {
                grid[i][j] = 1;
            }
        }
    }

}

void GameOfLifePTvec::printgrid(const char *filename) {
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

void GameOfLifePTvec::reset(uint32_t seed_new) {
    seed = seed_new;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 99);
    for (int i = 1; i < n - 1; ++i) {
        for (int j = 1; j < m - 1; ++j) {
            grid_tmp[i][j] = 0;
            if (dis(gen) < density) {
                grid[i][j] = 1;
            } else {
                grid[i][j] = 0;
            }
        }
    }
}

void GameOfLifePTvec::play(uint16_t numIter, bool printGrid, uint16_t numw) {

    nw = numw;
    std::vector<RANGE2> ranges(nw);
    uint32_t delta{(n - 2) / nw};
    std::vector<std::thread> tids;

    //compute the value of a single cell of position i j
    auto compute_cell = [&](int i, int j) {
        int16_t sum = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1]
                     + grid[i][j - 1] + grid[i][j + 1]
                     + grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];
        grid_tmp[i][j] = (sum == 3 || (sum == 2) & grid[i][j]);
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
#pragma GCC ivdep
                    for (int j = 1; j < m - 1; ++j) {
                        //compute_cell(i, j);
                        int16_t sum = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1]
                                     + grid[i][j - 1] + grid[i][j + 1]
                                     + grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];
                        int16_t a = (sum == 3) ;
                        int16_t b = (sum == 2);
                        int16_t c = grid[i][j];
                        int16_t d = b & c;
                        int16_t e = a || d;
                        grid_tmp[i][j] = e;

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

#endif //SPM_GAMEOFLIFEPTVEC_H
