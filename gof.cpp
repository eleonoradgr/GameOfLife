#include <iostream>
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePT.hpp"
#include "GameOfLifeOMP.hpp"
#include "GameOfLifePTRange2.hpp"
#include "GameOfLifePTCond.h"


int main(int argc, char *argv[] ){
    GameOfLifeSeq  game(5000,5000);
    game.play(25, false);

    std::cout << "Parallel Version" << std::endl;

    GameOfLifePT game2(5000, 5000);
    //game2.printgrid() ;
    game2.play(25, false, 25);
    //game2.printgrid( std::string("output.txt"));

    GameOfLifeOMP game3(5000, 5000);
    //game2.printgrid() ;
    game3.play(25, false,25);
    //game2.printgrid( std::string("output.txt"));

    GameOfLifePTRange2 game4 (5000, 5000);
    game4.play(25, false, 25);

    GameOfLifePTCond game5 (5000, 5000);
    game5.play(25, false, 25);
}
