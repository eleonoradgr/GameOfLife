#ifndef SPM_GAMEOFLIFESEQ_H
#define SPM_GAMEOFLIFESEQ_H

#include <vector>
#include <bitset>
#include <random>


class GameOfLifeSeq {
public:
    uint32_t n;
    uint32_t m;
    uint32_t seed;
    uint32_t density;
    std::vector<std::vector<bool> > grid;
    std::vector<std::vector<bool> > grid_tmp;

    explicit GameOfLifeSeq(uint32_t n = 30, uint32_t m = 30, uint32_t seed = 1234, float den = 0.3): n(n), m(m), seed(seed), density(den*100) , grid(n), grid_tmp(n){
        for (int i = 0 ; i< n; ++i){
            grid[i] = * new std::vector<bool>(m,false);
            grid_tmp[i] = * new std::vector<bool>(m,false);
        }

        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 99);
        for (int i = 1; i < n-1; ++i){
            for (int j = 1; j < m-1; ++j ){
                if (dis(gen) < density){
                    grid[i][j] = true;
                }
            }
        }

        printgrid();
    }

    void printgrid(){
        for (int i = 0; i <m; ++i){
            std::cout << "-";
        }
        std::cout << std::endl;
        for (int i = 0; i < n; ++i){
            for (int j = 0; j < m; ++j ){
                if (grid[i][j])
                    std::cout << "0";
                else
                    std::cout << " ";
            }
            std::cout << std::endl;
        }
        for (int i = 0; i <m; ++i){
            std::cout << "-";
        }
        std::cout << std::endl;
    }

    void reset(uint32_t seed_new = 1234){
        seed = seed_new;
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dis(0, 99);
        for (int i = 1; i < n-1; ++i){
            for (int j = 1; j < m-1; ++j ){
                grid_tmp[i][j] = false;
                if (dis(gen) < density){
                    grid[i][j] = true;
                }else{
                    grid[i][j] = false;
                }
            }
        }

        printgrid();
    }

    //an individual in a cell with 2 or  3 alive neighbours stays alive
    //an empty cell with exactly 3 alive neighbours becomes populated by a new individual
    //an alive cell with less than 2 alive neighbours dies (becomes empty)
    //an alive cell with more than 3 alive neighbours dies (become empty)

    void play(uint16_t numIter, bool printGrid = false){
        for (uint16_t iter = 0; iter < numIter; ++iter){
            for (int i = 1; i < n-1; ++i){
                for (int j = 1; j < m-1; ++j ){
                    int8_t sum = grid[i-1][j-1] + grid[i-1][j] + grid[i-1][j+1]
                                 + grid[i][j-1] + grid[i][j+1]
                                 + grid[i+1][j-1] + grid[i+1][j] + grid[i+1][j+1];
                    if(sum == 2){
                        grid_tmp[i][j] = grid[i][j];
                    }else if(sum == 3){
                        grid_tmp[i][j] = true;
                    }else{
                        grid_tmp[i][j] = false;
                    }
                }
            }
            grid.swap(grid_tmp);
            if (printGrid)
                printgrid();
        }

    }
};


#endif //SPM_GAMEOFLIFESEQ_H
