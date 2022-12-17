# CGOL-Pthread-Parallelization
Using pthreads to parallelize Conway's Game Of Life

# Usage
compile: make

execute:

Data parallelization: ./dat_gol numThreads gridSize numIterations (-d)

Task parallelization: ./task_gol gridSize numIterations (-d)

Where:
  - numThreads is the number of threads used for the data parallel program
  - gridSize is the size of the grid (i.e 10 = 10x10)
  - numIterations is the number of iterations of the board to be computed
  - if "id" is present, the output of each board is outputted to stdout

# Data Vs Task parallelization

Data approach: Assign n pthreads with an approximately equal number of board squares to compute and update.

Task approach: 3 threads.
  1. computes whether a square will be alive or dead in the next board iteration. Add the index of this square to either the alive queue or dead queue.
  2. Reads from the alive queue and updates the square for the next iteration
  3. Reads from the dead queue and updates the square for the next iteration

This approach is less effective than a serial verison. I just thought it would be interesting to create a task parallelized approach to this problem!

# Some of information
The board: The board is created with the odds of a dead cell being spawned: 66.66%, live cell 33.33%.

Due to the restraints of stdout terminal...printing boards too large (i.e 100 x 100) will look distored.
Boards up to ~60x60 appear nicely.

Maximum number of iterations = 10000
