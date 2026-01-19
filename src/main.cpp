#include "algorithms.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void printUsage(const char *progName) {
  std::cout << "Usage: " << progName << " <input_file>" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printUsage(argv[0]);
    return 1;
  }

  std::string inputFile = argv[1];

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

#ifdef RUN_BT
  sol = solver.solveBacktracking();
#elif defined(RUN_BB)
  sol = solver.solveBranchAndBound();
#elif defined(RUN_DP)
  sol = solver.solveDynamicProgramming();
#else
  std::cerr << "No strategy defined during compilation!" << std::endl;
  return 1;
#endif

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

  return 0;
}
