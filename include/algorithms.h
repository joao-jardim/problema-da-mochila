#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <chrono>
#include <string>
#include <vector>

struct Item {
  int id;
  int weight;
  int volume;
  int value;
};

struct Solution {
  int maxValue;
  std::vector<int>
      selectedItems; // 1 if selected, 0 otherwise (or just list of ids)
  double executionTime;
};

// Main solver class or functions
class KnapsackSolver {
public:
  struct Node {
    int level;
    int profit;
    int weight;
    int volume;
    float bound;
    std::vector<int> selection;
  };

  KnapsackSolver(int W, int V, const std::vector<Item> &items);

  Solution solveBacktracking();
  Solution solveBranchAndBound();
  Solution solveDynamicProgramming();

private:
  int W_limit;
  int V_limit;
  std::vector<Item> items;
  int n;

  // Backtracking helpers
  void backtracking(int idx, int currentW, int currentV, int currentValue,
                    std::vector<int> &currentSelection, Solution &bestSol);

  float calculateBound(Node u);
};

#endif
