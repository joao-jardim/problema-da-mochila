#include "branch_and_bound_solver.h"
#include <algorithm>
#include <chrono>
#include <queue>

BranchAndBoundSolver::BranchAndBoundSolver(
    int W, int V, const std::vector<Item> &items)
    : KnapsackSolver(W, V, items) {

    // Ordena itens por densidade (valor / (peso + volume))
    std::sort(this->items.begin(), this->items.end(),
        [](const Item &a, const Item &b) {
            return (float)a.value / (a.weight + a.volume) >
                   (float)b.value / (b.weight + b.volume);
        });
}

float BranchAndBoundSolver::calculateBound(const BBNode &u) const {
    if (u.weight >= W_limit || u.volume >= V_limit)
        return 0;

    float bound = u.profit;
    int currW = u.weight;
    int currV = u.volume;

    for (int i = u.level + 1; i < n; i++) {
        if (currW + items[i].weight <= W_limit &&
            currV + items[i].volume <= V_limit) {

            currW += items[i].weight;
            currV += items[i].volume;
            bound += items[i].value;

        } else {
            // Fração do item
            float w_frac = (float)(W_limit - currW) / items[i].weight;
            float v_frac = (float)(V_limit - currV) / items[i].volume;
            float frac = std::min(w_frac, v_frac);

            bound += items[i].value * frac;
            break;
        }
    }
    return bound;
}

Solution BranchAndBoundSolver::solve() {
    Solution sol;
    sol.maxValue = 0;
    sol.selectedItems.assign(n, 0);

    auto start = std::chrono::high_resolution_clock::now();

    std::priority_queue<BBNode, std::vector<BBNode>, CompareBound> pq;

    BBNode root;
    root.level = -1;
    root.profit = 0;
    root.weight = 0;
    root.volume = 0;
    root.selection.assign(n, 0);
    root.bound = calculateBound(root);

    pq.push(root);

    while (!pq.empty()) {
        BBNode u = pq.top();
        pq.pop();

        if (u.bound <= sol.maxValue)
            continue;

        BBNode v;
        v.level = u.level + 1;

        if (v.level >= n)
            continue;

        // ---- PEGAR ITEM ----
        v.weight = u.weight + items[v.level].weight;
        v.volume = u.volume + items[v.level].volume;
        v.profit = u.profit + items[v.level].value;
        v.selection = u.selection;
        v.selection[v.level] = 1;

        if (v.weight <= W_limit && v.volume <= V_limit) {
            if (v.profit > sol.maxValue) {
                sol.maxValue = v.profit;
                sol.selectedItems = v.selection;
            }

            v.bound = calculateBound(v);
            if (v.bound > sol.maxValue)
                pq.push(v);
        }

        // ---- NÃO PEGAR ITEM ----
        v.weight = u.weight;
        v.volume = u.volume;
        v.profit = u.profit;
        v.selection = u.selection;
        v.selection[v.level] = 0;
        v.bound = calculateBound(v);

        if (v.bound > sol.maxValue)
            pq.push(v);
    }

    auto end = std::chrono::high_resolution_clock::now();
    sol.executionTime =
        std::chrono::duration<double>(end - start).count();

    return sol;
}

