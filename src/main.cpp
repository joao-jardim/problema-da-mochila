#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "solver.h"
#ifdef RUN_BT
#include "backtracking_solver.h"
#endif
#ifdef RUN_BB
#include "branch_and_bound_solver.h"
#endif
#ifdef RUN_DP
#include "dynamic_programming_solver.h"
#endif

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

  //W = Quilos, V = Litros
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

  //unique_ptr destrói o objeto (solver) assim que sai do escopo (nao precisa de delete)
  std::unique_ptr<KnapsackSolver> solver;


//Aqui vamos instanciar o solver que vai ser usado, e chamar o construtor com as entradas
//que coletamos do arquivo.
//Solver é um ponteiro que aponta para a classe base, e é instanciado com a classe derivada.
#ifdef RUN_BT
  solver = std::make_unique<BacktrackingSolver>(W, V, items);
#elif defined(RUN_BB)
  solver = std::make_unique<BranchAndBoundSolver>(W, V, items);
#elif defined(RUN_DP)
  solver = std::make_unique<DynamicProgrammingSolver>(W, V, items);
#else
  std::cerr << "No strategy defined during compilation!" << std::endl;
  return 1;
#endif

  //solve é um método da classe derivada
  Solution sol = solver->solve();

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
