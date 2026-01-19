#include "branch_and_bound_solver.h"
#include <chrono>
#include <queue>

BranchAndBoundSolver::BranchAndBoundSolver(int W, int V,
                                           const std::vector<Item> &items)
    : KnapsackSolver(W, V, items) {}

float BranchAndBoundSolver::calculateBound(Node u) {
  if (u.weight >= W_limit || u.volume >= V_limit)
    return 0;

  float profit_bound = u.profit;
  int j = u.level + 1;

  for (int k = j; k < n; k++) {
    profit_bound += items[k].value;
  }
  return profit_bound;
}

Solution BranchAndBoundSolver::solve() {
  Solution sol;
  sol.maxValue = 0;
  sol.selectedItems.assign(n, 0);

  auto start = std::chrono::high_resolution_clock::now();

  std::queue<Node> Q; // BFS based B&B
  Node u, v;

  // Dummy root
  u.level = -1;
  u.profit = 0;
  u.weight = 0;
  u.volume = 0;
  u.selection.assign(n, 0);

  Q.push(u);

  while (!Q.empty()) {
    u = Q.front();
    Q.pop();

    if (u.level == n - 1)
      continue;

    v.level = u.level + 1;

    // Branch 1: Take item
    v.weight = u.weight + items[v.level].weight;
    v.volume = u.volume + items[v.level].volume;
    v.profit = u.profit + items[v.level].value;
    v.selection = u.selection;
    v.selection[v.level] = 1;

    if (v.weight <= W_limit && v.volume <= V_limit && v.profit > sol.maxValue) {
      sol.maxValue = v.profit;
      sol.selectedItems = v.selection;
    }

    float bound_take = calculateBound(v);
    if (bound_take > sol.maxValue)
      Q.push(v);

    // Branch 2: Don't take item
    v.weight = u.weight;
    v.volume = u.volume;
    v.profit = u.profit;
    v.selection = u.selection; // reset
    v.selection[v.level] = 0;

    float bound_skip = calculateBound(v);
    if (bound_skip > sol.maxValue)
      Q.push(v);
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  sol.executionTime = elapsed.count();

  return sol;
}
