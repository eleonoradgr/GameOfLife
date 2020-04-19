# GameOfLife

To implement a parallel version of Game of Life I’ve realized an abstract class GameOfLife and five different concrete extensions that differ one each other for the implementation of the method play. \
In particular:\
    • <b>GameOfLifeSeq</b> provides a trivial sequential implementation which scans all the cells of the grid through two nested for. The computed value is stored in a new grid and then grids are swapped. \
    • <b>GameOfLifePT</b>, a pool of thread is created according to the input parameter, and at each iteration, each thread computes the set of rows or part of it (if rows are less then workers), according to its concern. Two condition variables are used to coordinate operations of swap and passage to the next iteration. \
    • <b>GameOfLifePTfj</b>, it's just an experiment in which all threads are created at each iteration. \
    • <b>GameOfLifeOMP</b> provides the sequential implementation extended with "#pragma omp parallel for num_threads(nw) schedule(auto)" , so all is managed by OpenMP. \
    • <b>GameOfLifeOMPChunk</b>, similar to GameOfLifeOMP but I have fixed the chunk size and the schedule is static. 


Comparisons of Pthread implementation:

As expected the GameOfLifePT has better time performance then GameOfLifePTfj.
