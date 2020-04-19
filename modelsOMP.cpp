#include <iostream>
#include "utils/utimer.hpp"
#include "GameOfLifeSeq.hpp"
#include "GameOfLifeOMP.hpp"
#include "GameOfLifeOMPchunk.h"


int main(int argc, char *argv[]) {
    /*The speedup of two models are compared:
     * - GameOfLifeOMP where a parallel for pragma with auto scheduling is inserted before
     *      the two for of trivial (sequential) computation.
     * - GameOfLifeOMPchunk where a parallel for pragma with static scheduling and fixed chunck
     *      is inserted before the for which scans all the element of the grid.*/

    uint32_t n = 2000;
    uint32_t m = 2000;
    uint16_t iterations = 25;
    uint16_t repetitions = 20;
    uint32_t max_nw = 256;


    long time_tot = 0;
    for (int i = 0; i < repetitions; ++i) {
        GameOfLifeSeq game0(n, m);
        utimer seq(std::to_string(i) + " sequential ", &time_tot);
        game0.play(iterations, false);
    }
    time_tot = time_tot / repetitions;
    std::cout << n << "," << m << ",seq," << iterations << ",0," << time_tot << std::endl;

    for (int nw = 1; nw <= max_nw; ++nw) {
        long time_tot_omp = 0;
        for (int i = 0; i < repetitions; ++i) {
            GameOfLifeOMP game1(n, m);
            utimer omp(std::to_string(i) + " omp 1 ", &time_tot_omp);
            game1.play(iterations, false, nw);
        }
        time_tot_omp = time_tot_omp / repetitions;
        float speedup = float(time_tot) / time_tot_omp;
        std::cout << n << "," << m << ",omp1," << iterations << "," + std::to_string(nw) + "," << time_tot_omp << ","
                  << speedup << std::endl;
    }

    for (int nw = 1; nw <= max_nw; ++nw) {
        long time_tot_omp = 0;
        for (int i = 0; i < repetitions; ++i) {
            GameOfLifeOMPchunk game2(n, m);
            utimer omp(std::to_string(i) + " omp 2 ", &time_tot_omp);
            game2.play(iterations, false, nw);
        }
        time_tot_omp = time_tot_omp / repetitions;
        float speedup = float(time_tot) / time_tot_omp;
        std::cout << n << "," << m << ",omp2," << iterations << "," + std::to_string(nw) + "," << time_tot_omp << ","
                  << speedup << std::endl;
    }
}


