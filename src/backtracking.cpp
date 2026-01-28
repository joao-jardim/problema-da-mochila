#include "backtracking_solver.h"
#include <chrono>



BacktrackingSolver::BacktrackingSolver(int W, int V,
                                       const std::vector<Item> &items)
    : KnapsackSolver(W, V, items) {}

Solution BacktrackingSolver::solve() {
  Solution sol;
  sol.maxValue = 0;
  sol.selectedItems.resize(n, 0);
  std::vector<int> currentSelection(n, 0); 

  auto start = std::chrono::high_resolution_clock::now();

  backtrack(currentSelection, -1, sol);

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  sol.executionTime = elapsed.count();

  return sol;
}

void BacktrackingSolver::backtrack(std::vector<int> &a, int k, Solution &bestSol) {
  const int MAXCANDIDATES = 2;
  int c[MAXCANDIDATES]; /* candidatos para a prox posição */
  int ncandidates;      /* numero de candidatos */
  int i;                /* contador */

  if (is_a_solution(k)) {
    process_solution(a, k, bestSol);
  } else {
    k = k + 1;
    construct_candidates(a, k, c, &ncandidates);
    for (i = 0; i < ncandidates; i++) {
      a[k] = c[i];
      backtrack(a, k, bestSol);
    }
  }
}

bool BacktrackingSolver::is_a_solution(int k) {
  return (k == n - 1);
}

void BacktrackingSolver::process_solution(const std::vector<int> &a, int k,
                                          Solution &bestSol) {
  int currentW = 0;
  int currentV = 0;
  int currentValue = 0;

  // calc peso, volume e valor da solução atual
  for (int i = 0; i <= k; i++) {
    if (a[i] == 1) {
      currentW += items[i].weight;
      currentV += items[i].volume;
      currentValue += items[i].value;
    }
  }

  // verifica se é uma solução viável
  if (currentW <= W_limit && currentV <= V_limit) { 
    // se for melhor que a melhor encontrada até agora, atualiza
    if (currentValue > bestSol.maxValue) {
      bestSol.maxValue = currentValue;
      bestSol.selectedItems = a;
    }
  }
}

void BacktrackingSolver::construct_candidates(const std::vector<int> &a, int k,
                                              int c[], int *ncandidates) {
  *ncandidates = 0;

  // otimização: podar se já excedeu limites antes mesmo de tentar adicionar?
  // na verdade, construct_candidates deve gerar possíveis valores para a posição k.
  // para mochila binária, os candidatos são 1 (levar) ou 0 (não levar).

  int currentW = 0;
  int currentV = 0;
  // calcula estado atual até k-1
  for (int i = 0; i < k; i++) {
    if (a[i] == 1) {
      currentW += items[i].weight;
      currentV += items[i].volume;
    }
  }

  // tenta incluir o item k (candidato 1)
  // verifica restrição
  if (currentW + items[k].weight <= W_limit && 
      currentV + items[k].volume <= V_limit) {
    c[*ncandidates] = 1;
    (*ncandidates)++;
  }

  // sempre é possível não incluir o item k (candidato 0)
  c[*ncandidates] = 0;
  (*ncandidates)++;
}
