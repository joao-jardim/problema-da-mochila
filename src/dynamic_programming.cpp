#include "dynamic_programming_solver.h"
#include <chrono>
#include <vector>

//construtor
DynamicProgrammingSolver::DynamicProgrammingSolver(
    int W, int V, const std::vector<Item> &items)
    : KnapsackSolver(W, V, items) {}

Solution DynamicProgrammingSolver::solve() {
  Solution sol;
  sol.maxValue = 0;

  auto start = std::chrono::high_resolution_clock::now();

  //inicializa a tabela de memoização com 0
  int ***table = new int **[n + 1];
  for (int i = 0; i <= n; ++i) {
    table[i] = new int *[W_limit + 1];
    for (int w = 0; w <= W_limit; ++w) {
      table[i][w] = new int[V_limit + 1];
      for (int v = 0; v <= V_limit; ++v)
        table[i][w][v] = 0;
    }
  }

  //n = itens
  for (int i = 1; i <= n; i++) {
    //pegando os atributos de um item
    int w_item = items[i - 1].weight;
    int v_item = items[i - 1].volume;
    int val_item = items[i - 1].value;

    //percorre todas as capacidades de peso e volume
    for (int w = 0; w <= W_limit; w++) {
      for (int v = 0; v <= V_limit; v++) {

        //escolhemos nao colocar o item, entao na tabela de memoização
        //vamos só copiar os mesmos valores
        table[i][w][v] = table[i - 1][w][v];

        //escolhemos colocar o item
        if (w >= w_item && v >= v_item) {
          //valor total vai ser o valor do item mais o valor da tabela de memoização
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
  //começamos pelo ultimo item
  int currW = W_limit;
  int currV = V_limit;

  for (int i = n; i > 0; i--) {
    //se o valor for igual nao pegamos o item
    if (table[i][currW][currV] != table[i - 1][currW][currV]) {
      sol.selectedItems[i - 1] = 1;
      currW -= items[i - 1].weight;
      currV -= items[i - 1].volume;
    }
  }

  //limpeza de memoria
  for (int i = 0; i <= n; ++i) {
    for (int w = 0; w <= W_limit; ++w) {
      delete[] table[i][w];
    }
    delete[] table[i];
  }
  delete[] table;

  //tempo de execução
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  sol.executionTime = elapsed.count();

  return sol;
}
