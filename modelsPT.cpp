
#include <iostream>
#include "utils/utimer.hpp"
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePTfj.hpp"
#include "GameOfLifePT.hpp"


int main(int argc, char *argv[]) {

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
        long time_tot_pt = 0;
        for (int i = 0; i < repetitions; ++i) {
            GameOfLifePT game1(n, m);
            utimer omp(std::to_string(i) + " PT ", &time_tot_pt);
            game1.play(iterations, false, nw);
        }
        time_tot_pt = time_tot_pt / repetitions;
        float speedup = float(time_tot) / time_tot_pt;
        std::cout << n << "," << m << ",PT," << iterations << "," + std::to_string(nw) + "," << time_tot_pt << ","
                  << speedup << std::endl;
    }

    for (int nw = 1; nw <= max_nw; ++nw) {
        long time_tot_pt = 0;
        for (int i = 0; i < repetitions; ++i) {
            GameOfLifePTfj game2(n, m);
            utimer omp(std::to_string(i) + " PTfj ", &time_tot_pt);
            game2.play(iterations, false, nw);
        }
        time_tot_pt = time_tot_pt / repetitions;
        float speedup = float(time_tot) / time_tot_pt;
        std::cout << n << "," << m << ",PTfj," << iterations << "," + std::to_string(nw) + "," << time_tot_pt << ","
                  << speedup << std::endl;
    }
}


