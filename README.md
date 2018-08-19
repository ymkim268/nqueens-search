# nqueens-search

## problem statement:
* similar to n-queens problem
* bfs, dfs, sa (simulated annealing) implemenations
* given an arrangement of trees, find new arrangement of lizards such that no lizard eat another
* directions of lizards equivalent a queen on chess board

## input.txt format:
* 1st line: selected one of BFS, DFS, SA
* 2nd line: n x n arrangement (width x height) square
* 3rd line: p, num of lizards (queens)
* n lines: n x n board, 0 nothing, 2 tree (fixed)

## output.txt:
* 1st line: OK or FAIL (solution found or not)
* n lines: n x n nursery, including lizards (1), trees (2), nothing (0)