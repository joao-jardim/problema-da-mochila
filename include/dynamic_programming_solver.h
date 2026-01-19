#ifndef DYNAMIC_PROGRAMMING_SOLVER_H
#define DYNAMIC_PROGRAMMING_SOLVER_H

#include "solver.h"

class DynamicProgrammingSolver : public KnapsackSolver {
public:
  DynamicProgrammingSolver(int W, int V, const std::vector<Item> &items);
  Solution solve() override;
};

#endif
