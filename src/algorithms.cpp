#include "algorithms.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>

KnapsackSolver::KnapsackSolver(int W, int V, const std::vector<Item> &items)
    : W_limit(W), V_limit(V), items(items), n(items.size()) {}

// ==========================================
// BACKTRACKING
// ==========================================

void KnapsackSolver::backtracking(int idx, int currentW, int currentV,
                                  int currentValue,
                                  std::vector<int> &currentSelection,
                                  Solution &bestSol) {
  // Pruning
  if (currentW > W_limit || currentV > V_limit) {
    return;
  }

  // Update best solution found so far
  if (currentValue > bestSol.maxValue) {
    bestSol.maxValue = currentValue;
    bestSol.selectedItems = currentSelection;
  }

  // Base case
  if (idx == n) {
    return;
  }

  // Recursive step 1: Include item[idx]
  if (currentW + items[idx].weight <= W_limit &&
      currentV + items[idx].volume <= V_limit) {
    currentSelection[idx] = 1;
    backtracking(idx + 1, currentW + items[idx].weight,
                 currentV + items[idx].volume, currentValue + items[idx].value,
                 currentSelection, bestSol);
    currentSelection[idx] = 0; // Backtrack
  }

  // Recursive step 2: Exclude item[idx]
  // Optimization: Bound check could go here (if potential remaining value <
  // bestSol, prune) For this assignment, basic backtracking is usually
  // sufficient, but we can add upper bound pruning for better performance if
  // needed. Let's implement basic full search first for correctness.
  backtracking(idx + 1, currentW, currentV, currentValue, currentSelection,
               bestSol);
}

Solution KnapsackSolver::solveBacktracking() {
  Solution sol;
  sol.maxValue = 0;
  sol.selectedItems.resize(n, 0);
  std::vector<int> currentSelection(n, 0);

  auto start = std::chrono::high_resolution_clock::now();

  backtracking(0, 0, 0, 0, currentSelection, sol);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  sol.executionTime = elapsed.count();

  return sol;
}

// ==========================================
// BRANCH AND BOUND
// ==========================================

float KnapsackSolver::calculateBound(Node u) {
  if (u.weight >= W_limit || u.volume >= V_limit)
    return 0;

  float profit_bound = u.profit;
  int j = u.level + 1;

  // Greedy strategy: fill with remaining items sorted by ratio
  // Ideally items should be sorted by value/weight or something similar for B&B
  // to work best. For this implementation, we will assume the order provided or
  // just iterate. A proper B&B sorts items by v/w or v/l. Let's do a simple
  // bound summing remaining: NOTE: For effective B&B, initial sorting is
  // crucial. We will create a local copy of items sorted by value/weight.

  // However, since we have 2 constraints, sorting is tricky.
  // Let's use a simple relaxation: treat fractional knapsack for 2 constraints?
  // Harder. Let's stick to the simpler bound: value so far + sum of all
  // remaining positive items (Upper Bound). This is a loose bound but valid.

  for (int k = j; k < n; k++) {
    profit_bound += items[k].value;
  }
  return profit_bound;
}

// Comparison for Priority Queue
struct CompareNode {
  bool operator()(const KnapsackSolver::Node &a,
                  const KnapsackSolver::Node &b) {
    return a.profit < b.profit; // Max heap based on profit (or bound?)
    // Standard B&B uses Best-First Search often based on Bound.
    // Let's use Bound.
    // return a.bound < b.bound;
  }
};

Solution KnapsackSolver::solveBranchAndBound() {
  // Note: B&B effectiveness heavily depends on sorting items.
  // We will proceed without reordering for simplicity unless performance is
  // terrible.

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

    // Add to queue if bound is promising
    // Here we use a very simple 'promising' check: can we even possibly beat
    // max? Bound = current_profit + remaining_possible_value
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

// ==========================================
// DYNAMIC PROGRAMMING
// ==========================================

Solution KnapsackSolver::solveDynamicProgramming() {
  Solution sol;
  sol.maxValue = 0;

  auto start = std::chrono::high_resolution_clock::now();

  // dp[k][w][v] = max value using first k items with weight w and volume v
  // Memory optimization: We only need previous layer.
  // dp[w][v]

  std::vector<std::vector<int>> dp(W_limit + 1,
                                   std::vector<int>(V_limit + 1, 0));
  // To reconstruct solution, we need a 3D table or a way to backtrack.
  // Since we need to output selected items, we usually need the full table or a
  // bitset. Given the constraints in typical assignments, full table might be
  // too big if W,V are large. But let's try strict 2D array + path
  // reconstruction logic (requires full history) OR straightforward 3D array?

  // Let's implement full 3D extraction for correctness.
  // dp[item][weight][volume]

  // If n*W*V is too large (e.g. 100 * 500 * 500 = 25M ints = 100MB), it's fine
  // for modern PC.

  // Use flat vector or vector of vectors? vector<vector<vector>> is slow.
  // Let's use a linearized approach or the vector structure.

  // Initialize
  // Using heap allocation slightly better managed
  int ***table = new int **[n + 1];
  for (int i = 0; i <= n; ++i) {
    table[i] = new int *[W_limit + 1];
    for (int w = 0; w <= W_limit; ++w) {
      table[i][w] = new int[V_limit + 1];
      // Initialize with 0
      for (int v = 0; v <= V_limit; ++v)
        table[i][w][v] = 0;
    }
  }

  for (int i = 1; i <= n; i++) {
    int w_item = items[i - 1].weight;
    int v_item = items[i - 1].volume;
    int val_item = items[i - 1].value;

    for (int w = 0; w <= W_limit; w++) {
      for (int v = 0; v <= V_limit; v++) {
        // Default: don't include
        table[i][w][v] = table[i - 1][w][v];

        if (w >= w_item && v >= v_item) {
          int val_with = table[i - 1][w - w_item][v - v_item] + val_item;
          if (val_with > table[i][w][v]) {
            table[i][w][v] = val_with;
          }
        }
      }
    }
  }

  sol.maxValue = table[n][W_limit][V_limit];

  // Backtrack to find items
  sol.selectedItems.assign(n, 0);
  int currW = W_limit;
  int currV = V_limit;
  for (int i = n; i > 0; i--) {
    if (table[i][currW][currV] != table[i - 1][currW][currV]) {
      // Item was selected
      sol.selectedItems[i - 1] = 1;
      currW -= items[i - 1].weight;
      currV -= items[i - 1].volume;
    }
  }

  // Clean up
  for (int i = 0; i <= n; ++i) {
    for (int w = 0; w <= W_limit; ++w) {
      delete[] table[i][w];
    }
    delete[] table[i];
  }
  delete[] table;

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  sol.executionTime = elapsed.count();

  return sol;
}
