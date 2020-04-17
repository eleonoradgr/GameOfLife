#include <iostream>
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePT.hpp"
#include "GameOfLifeOMP.hpp"
#include "GameOfLifePTRange2.hpp"
#include "GameOfLifePTCond.hpp"
#include "GameOfLifeOMPchunk.h"


int main(int argc, char *argv[] ){
    /*GameOfLifeSeq  game(5000,5000);
    game.play(25, false);

    std::cout << "Parallel Version" << std::endl;

    GameOfLifePT game2(5000, 5000);
    game2.play(25, false, 25);*/

    GameOfLifeOMP game3(10000, 5000);
    game3.play(25, false,25);

   /* GameOfLifePTRange2 game4 (5000, 5000);
    game4.play(25, false, 25);

    GameOfLifePTCond game5 (5000, 5000);
    game5.play(25, false, 25);*/

    GameOfLifeOMPchunk game6(50, 50);
    game6.play(25, false,25);
}
