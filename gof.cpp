#include <iostream>
#include "GameOfLifeSeq.hpp"


int main(int argc, char *argv[] ){
    GameOfLifeSeq game(15,60);
    game.reset();
    game.reset(5678);
    game.play(10, true);
}
