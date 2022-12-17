# CGOL-Pthread-Parallelization
Using pthreads to parallelize Conway's Game Of Life

# Usage
compile: make

exec:
Data parallelization: ./dat_gol numThreads gridSize numIterations (-d)

Task parallelization: ./task_gol gridSize numIterations (-d)

# Some of information
The board: The board is created with the odds of a dead cell being spawned: 66.66%, live cell 33.33%.

Due to the restraints of stdout terminal...printing boards too large (i.e 100 x 100) will look distored.
Boards up to ~60x60 appear nicely.

Maximum number of iterations = 10000
