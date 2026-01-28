#ifndef BACKTRACKING_SOLVER_H
#define BACKTRACKING_SOLVER_H

#include "solver.h"

class BacktrackingSolver : public KnapsackSolver {
public:
  BacktrackingSolver(int W, int V, const std::vector<Item> &items);
  Solution solve() override;

private:
  void backtrack(std::vector<int> &a, int k, Solution &bestSol);
  bool is_a_solution(int k);
  void process_solution(const std::vector<int> &a, int k, Solution &bestSol);
  void construct_candidates(const std::vector<int> &a, int k, int c[], int *ncandidates);
};

#endif
