<div align="center">

# GameOfLife
</div>

To implement a parallel version of Game of Life I’ve realized an abstract class GameOfLife and five different concrete extensions that differ one each other for the implementation of the method play. \
In particular:\
    • <b>GameOfLifeSeq</b> provides a trivial sequential implementation which scans all the cells of the grid through two nested for. The computed value is stored in a new grid and then grids are swapped. \
    • <b>GameOfLifePT</b>, a pool of thread is created according to the input parameter, and at each iteration, each thread computes the set of rows or part of it (if rows are less then workers), according to its concern. Two condition variables are used to coordinate operations of swap and passage to the next iteration. \
    • <b>GameOfLifePTfj</b>, it's just an experiment in which all threads are created at each iteration. \
    • <b>GameOfLifeOMP</b> provides the sequential implementation extended with "#pragma omp parallel for num_threads(nw) schedule(auto)" , so all is managed by OpenMP. \
    • <b>GameOfLifeOMPChunk</b>, similar to GameOfLifeOMP but I have fixed the chunk size and the schedule is static. 


<div align="center">
<b>Comparisons of Pthread implementation</b>
</div>

As expected the GameOfLifePT has better time performance then GameOfLifePTfj.
![PTcomparison](https://github.com/eleonoradgr/GameOfLife/blob/master/img/ptModelsTime.png)

Speedup is linear up to about 15 workers, it slowly increases up to about 50 workers and then decrease with an interesting pattern which I’m still investigating.
![PTcomparison1](https://github.com/eleonoradgr/GameOfLife/blob/master/img/ptModelsSpeedup.png)

Scalability follows the same trend of speedup, indeed there is a little difference between the sequential time and the time achieved with one thread.
![PTcomparison2](https://github.com/eleonoradgr/GameOfLife/blob/master/img/ptModelsScalabiity.png)


<div align="center">
<b>Comparison of OMP implementation</b>
</div>

GameOfLifeOMP has better time performance then GameOfLifeOMPChunk, due to the fact that the computation of each task is less heavy.
![OMPcomparison](https://github.com/eleonoradgr/GameOfLife/blob/master/img/ompModelsTime.png)

Speedup and Scalability graphs follow.
![OMPcomparison1](https://github.com/eleonoradgr/GameOfLife/blob/master/img/ompModelsSpeedup.png)
![OMPcomparison2](https://github.com/eleonoradgr/GameOfLife/blob/master/img/ompModelsScalabiity.png)

<div align="center">
<b>Code</b>
</div>

Code of classes is available attached to this file, to run an example with GameOfLifeSeq, GameOfLifePT and GameOfLifeOMP just compile gof.cpp file.
Parameters needed are:\
    • Number of iteration (mandatory)\
    • Random number generator seed (mandatory)\
    • Number of rows (mandatory)\
    • Number of columns (mandatory)\
    • Parallelism degree (mandatory)\
    • Densitity of grid (default value 0.3)\
    • Print the grid after each iteration (default value false)\
    • Print the grid at the end of all the iterations (default value false)

All the experiments have been runned on Xeon PHI server after compiling with g++ 9.2.0 version and the following flag : -fopenmp -O3 - DNDEBUG -ptrhead.

