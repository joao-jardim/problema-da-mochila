#ifndef SOLVER_H
#define SOLVER_H

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
  std::vector<int> selectedItems; // 1 if selected, 0 otherwise
  double executionTime;
};

class KnapsackSolver {
public:
  KnapsackSolver(int W, int V, const std::vector<Item> &items);
  virtual ~KnapsackSolver() = default;

  virtual Solution solve() = 0;

protected:
  int W_limit;
  int V_limit;
  std::vector<Item> items;
  int n;
};

#endif
