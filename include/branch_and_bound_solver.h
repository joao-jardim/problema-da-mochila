#ifndef BRANCH_AND_BOUND_SOLVER_H
#define BRANCH_AND_BOUND_SOLVER_H

#include "solver.h"
#include <vector>

class BranchAndBoundSolver : public KnapsackSolver {
public:
  // Helper struct for B&B nodes
  struct Node {
    int level;
    int profit;
    int weight;
    int volume;
    float bound;
    std::vector<int> selection;
  };

  BranchAndBoundSolver(int W, int V, const std::vector<Item> &items);
  Solution solve() override;

private:
  float calculateBound(Node u);
};

#endif
