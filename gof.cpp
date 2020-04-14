#include <iostream>
#include "GameOfLifeSeq.hpp"
#include "GameOfLifePT.hpp"
#include "GameOfLifeOMP.hpp"


int main(int argc, char *argv[] ){
    GameOfLifeSeq  game(30000,600);
    game.play(30, false);

    std::cout << "Parallel Version" << std::endl;

    GameOfLifePT game2(30000, 600);
    //game2.printgrid() ;
    game2.play(30, false, 20);
    //game2.printgrid( std::string("output.txt"));

    GameOfLifeOMP game3(30000, 600);
    //game2.printgrid() ;
    game3.play(30, false,20);
    //game2.printgrid( std::string("output.txt"));


}
