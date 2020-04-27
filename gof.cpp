#include <iostream>
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePTfj.hpp"
#include "GameOfLifePT.hpp"
#include "GameOfLifeOMP.hpp"
#include "GameOfLifeOMPchunk.h"
#include "GameOfLifePTvec.hpp"


int main(int argc, char *argv[]) {

    if (argc < 6) {
        std::cout << "usage: " << argv[0] << std::endl;
        std::cout << "iter : Number of iteration (mandatory)" << std::endl;
        std::cout << "seed : Random number generator seed (mandatory)" << std::endl;
        std::cout << "n : Number of rows (mandatory)" << std::endl;
        std::cout << "m : Number of columns (mandatory)" << std::endl;
        std::cout << "nw : Parallelism degree (mandatory)" << std::endl;
        std::cout << "density : Density of grid (default value 0.3)" << std::endl;
        std::cout << "print_grid : Print the grid after each iteration (default value false)" << std::endl;
        std::cout << "print_res : Print the grid at the end of all the iterations (default value false)" << std::endl;
        return -1;
    }

    uint16_t iterations = atoi(argv[1]);;
    uint32_t seed = atoi(argv[2]);;
    uint32_t n = atoi(argv[3]); // number of rows
    uint32_t m = atoi(argv[4]); // number of columns
    uint32_t nw = atoi(argv[5]);
    float den = 0.3;
    if (argc >= 7) {
        den = atof(argv[6]);
    }
    bool print_grid = false;
    if (argc >= 8) {
        if (argv[7] == "true" || argv[7] == "1")
            print_grid = true;
    }
    bool print_res = false;
    if (argc >= 9) {
        if (argv[8] == "true" || atoi(argv[8]) == 1)
            print_res = true;
    }


    GameOfLifeSeq game(n, m, seed, den);
    {
        utimer seq(" Sequential execution ");
        game.play(iterations, print_grid);
    }
    if (print_res)
        game.printgrid();


    GameOfLifePT game1(n, m, seed, den);
    {
        utimer pt(" Parallel pthread execution");
        game1.play(iterations, print_grid, nw);
    }
    if (print_res)
        game1.printgrid();

    GameOfLifePTvec game2(n, m, seed, den);
    {
        utimer pt(" Parallel pthread vectorized execution");
        game2.play(iterations, print_grid, nw);
    }
    if (print_res)
        game2.printgrid();

    GameOfLifeOMP game3(n, m, seed, den);
    {
        utimer omp(" Paralel omp execution ");
        game3.play(iterations, print_grid, nw);
    }
    if (print_res)
        game3.printgrid();

}
