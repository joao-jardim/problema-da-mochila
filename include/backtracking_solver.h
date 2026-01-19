#ifndef BACKTRACKING_SOLVER_H
#define BACKTRACKING_SOLVER_H

#include "solver.h"

class BacktrackingSolver : public KnapsackSolver {
public:
  BacktrackingSolver(int W, int V, const std::vector<Item> &items);
  Solution solve() override;

private:
  void backtracking(int idx, int currentW, int currentV, int currentValue,
                    std::vector<int> &currentSelection, Solution &bestSol);
};

#endif
