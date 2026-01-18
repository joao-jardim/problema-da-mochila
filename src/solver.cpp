#include "algorithms.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void printUsage() {
  std::cout << "Usage: ./solver <strategy> <input_file>" << std::endl;
  std::cout << "Strategies: BT (Backtracking), BB (Branch & Bound), DP "
               "(Dynamic Programming)"
            << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printUsage();
    return 1;
  }

  std::string strategy = argv[1];
  std::string inputFile = argv[2];

  std::ifstream infile(inputFile);
  if (!infile.is_open()) {
    std::cerr << "Error opening file: " << inputFile << std::endl;
    return 1;
  }

  int W, V;
  if (!(infile >> W >> V)) {
    std::cerr << "Error reading W and V" << std::endl;
    return 1;
  }

  std::vector<Item> items;
  int w, l, v_val;
  int id = 0;
  while (infile >> w >> l >> v_val) {
    items.push_back({id++, w, l, v_val});
  }
  infile.close();

  KnapsackSolver solver(W, V, items);
  Solution sol;

  if (strategy == "BT") {
    sol = solver.solveBacktracking();
  } else if (strategy == "BB") {
    sol = solver.solveBranchAndBound();
  } else if (strategy == "DP") {
    sol = solver.solveDynamicProgramming();
  } else {
    std::cerr << "Unknown strategy: " << strategy << std::endl;
    printUsage();
    return 1;
  }

  // Output format:
  // MaxValue
  // Selected Items (0 or 1) ... ? Or IDs?
  // Requirement says: "juntamente com os itens que foram colocados na mochila"
  // Let's print the count, then the indices (1-based or 0-based), or just the
  // list. Example output doesn't specify exact format, so let's be clear.

  std::cout << "Max Value: " << sol.maxValue << std::endl;
  std::cout << "Time: " << sol.executionTime << "s" << std::endl;
  std::cout << "Selected Items: ";
  bool first = true;
  for (size_t i = 0; i < sol.selectedItems.size(); ++i) {
    if (sol.selectedItems[i]) {
      if (!first)
        std::cout << " ";
      std::cout << i; // Printing 0-based index
      first = false;
    }
  }
  std::cout << std::endl;

  // Also printing a CSV-friendly line to stderr for easier benchmarking parsing
  // if needed format: strategy, n, W, V, execution_time, max_value std::cerr <<
  // strategy << "," << items.size() << "," << W << "," << V << "," <<
  // sol.executionTime << "," << sol.maxValue << std::endl;

  return 0;
}
