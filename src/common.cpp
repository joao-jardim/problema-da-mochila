#include "algorithms.h"

KnapsackSolver::KnapsackSolver(int W, int V, const std::vector<Item> &items)
    : W_limit(W), V_limit(V), items(items), n(items.size()) {}
