#include <iostream>
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePT.hpp"


int main(int argc, char *argv[] ){
    GameOfLifeSeq game(1500,6000);
    //game.printgrid() ;
    game.play(20, false);
    //game.printgrid() ;

    std::cout << "Parallel Version" << std::endl;

    GameOfLifePT game2(1500, 6000);
    //game2.printgrid() ;
    game2.play(20, 20, false);
    //game2.printgrid( std::string("output.txt"));
}
