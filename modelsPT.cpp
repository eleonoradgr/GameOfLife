
#include <iostream>
#include "utils/utimer.hpp"
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePT.hpp"
#include "GameOfLifePTCond.hpp"
#include "GameOfLifePTRange2.hpp"



int main(int argc, char *argv[]) {
    /*The speedup of two models are compared:
     * - GameOfLifeOMP where a parallel for pragma with auto scheduling is inserted before
     *      the two for of trivial (sequential) computation.
     * - GameOfLifeOMPchunk where a parallel for pragma with static scheduling and fixed chunck
     *      is inserted before the for which scans all the element of the grid.*/

    long time_tot = 0 ;
    for (int i = 0; i < 20; ++i) {
        GameOfLifeSeq game0(1000, 1000);
        utimer seq(std::to_string(i) + " sequential ", &time_tot);
        game0.play(25, false);
    }
    time_tot = time_tot/20;
    std::cout << "1000,1000,seq,25,0,"<< time_tot << std::endl;

    for (int nw = 1; nw <= 256; ++nw) {
        long time_tot_pt= 0 ;
        for (int i = 0; i < 20; ++i) {
            GameOfLifeOMP game1(1000, 1000);
            utimer omp(std::to_string(i) + " PTrange ", &time_tot_omp);
            game1.play(25, false, nw);
        }
        time_tot_pt = time_tot_pt/20;
        float speedup =  float(time_tot) /time_tot_pt;
        std::cout << "2000,2000,PTrange,25," + std::to_string(nw) + ","<< time_tot_pt<< ","<< speedup << std::endl;
    }

    for (int nw = 1; nw <= 256; ++nw) {
        long time_tot_pt = 0 ;
        for (int i = 0; i < 20; ++i) {
            GameOfLifePT game2(1000, 1000);
            utimer omp(std::to_string(i) + " PT ", &time_tot_omp);
            game2.play(25, false, nw);
        }
        time_tot_omp = time_tot_omp/20;
        float speedup =  float(time_tot) /time_tot_pt;
        std::cout << "1000,1000,PT,25," + std::to_string(nw) + ","<< time_tot_pt<< ","<< speedup << std::endl;
    }

    for (int nw = 1; nw <= 256; ++nw) {
        long time_tot_pt = 0 ;
        for (int i = 0; i < 20; ++i) {
            GameOfLifePTCond game2(1000, 1000);
            utimer omp(std::to_string(i) + " PTcond ", &time_tot_omp);
            game2.play(25, false, nw);
        }
        time_tot_omp = time_tot_omp/20;
        float speedup =  float(time_tot) /time_tot_pt;
        std::cout << "1000,1000,PTcond,25," + std::to_string(nw) + ","<< time_tot_pt<< ","<< speedup << std::endl;
    }
}


