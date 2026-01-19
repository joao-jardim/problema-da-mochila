#include "algorithms.h"
#include <chrono>
#include <vector>

Solution KnapsackSolver::solveDynamicProgramming() {
  Solution sol;
  sol.maxValue = 0;

  auto start = std::chrono::high_resolution_clock::now();

  // Initialize 3D table
  int ***table = new int **[n + 1];
  for (int i = 0; i <= n; ++i) {
    table[i] = new int *[W_limit + 1];
    for (int w = 0; w <= W_limit; ++w) {
      table[i][w] = new int[V_limit + 1];
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
