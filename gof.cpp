#include <iostream>
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePT.hpp"
#include "GameOfLifeOMP.hpp"
#include "GameOfLifePTRange2.hpp"


int main(int argc, char *argv[] ){
    GameOfLifeSeq  game(5,20000);
    game.play(15, false);

    std::cout << "Parallel Version" << std::endl;

    GameOfLifePT game2(5, 20000);
    //game2.printgrid() ;
    game2.play(15, false, 4);
    //game2.printgrid( std::string("output.txt"));

    GameOfLifeOMP game3(5, 20000);
    //game2.printgrid() ;
    //game3.play(15, false,4);
    //game2.printgrid( std::string("output.txt"));

    GameOfLifePTRange2 game4 (5, 20000);
    game4.play(15, false, 4);


}
