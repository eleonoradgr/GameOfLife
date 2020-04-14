#include <iostream>
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePT.hpp"
#include "GameOfLifeOMP.hpp"


int main(int argc, char *argv[] ){
    GameOfLifeSeq game(30000,6000);
    //game.printgrid() ;
    game.play(20, false);
    //game.printgrid() ;

    std::cout << "Parallel Version" << std::endl;

    GameOfLifePT game2(30000, 6000);
    //game2.printgrid() ;
    game2.play(20, 40, false);
    //game2.printgrid( std::string("output.txt"));

    GameOfLifeOMP game3(30000, 6000);
    //game2.printgrid() ;
    game3.play(20, 40, false);
    //game2.printgrid( std::string("output.txt"));


}
