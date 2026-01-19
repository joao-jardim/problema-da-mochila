#include "backtracking_solver.h"
#include <chrono>

BacktrackingSolver::BacktrackingSolver(int W, int V,
                                       const std::vector<Item> &items)
    : KnapsackSolver(W, V, items) {}

void BacktrackingSolver::backtracking(int idx, int currentW, int currentV,
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
  backtracking(idx + 1, currentW, currentV, currentValue, currentSelection,
               bestSol);
}

Solution BacktrackingSolver::solve() {
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
