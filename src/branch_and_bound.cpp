// Branch and Bound é testar possibilidades, mas parar cedo quando dá pra 
// provar que nao vale a pena continuar.

#include "branch_and_bound_solver.h"
#include <algorithm>
#include <chrono>
#include <queue>

BranchAndBoundSolver::BranchAndBoundSolver(
    int W, int V, const std::vector<Item> &items)
    : KnapsackSolver(W, V, items) {

    // Ordena itens por densidade (valor / (peso + volume)) - Itens mais vantajosos primeiro
    // Se os melhores vem primeiro, o bound fica mais justo -> poda melhor
    std::sort(this->items.begin(), this->items.end(),
        [](const Item &a, const Item &b) {
            return (float)a.value / (a.weight + a.volume) >
                   (float)b.value / (b.weight + b.volume);
        });
}

// Se pudesse trapazear e pegar fracoens dos proximos itens, qual seria o maior valor que ainda da pra atingir?
float BranchAndBoundSolver::calculateBound(const BBNode &u) const {
    if (u.weight >= W_limit || u.volume >= V_limit)
        return 0;

    // Comeca com o que ja tem
    float bound = u.profit;
    int currW = u.weight;
    int currV = u.volume;

    // Tenta pegar os proximos itens inteiros
    for (int i = u.level + 1; i < n; i++) {
        // Se couber -> pega tudo
        if (currW + items[i].weight <= W_limit &&
            currV + items[i].volume <= V_limit) {

            currW += items[i].weight;
            currV += items[i].volume;
            bound += items[i].value;
        
        // Quando nao couber, pega so uma fracao do item
        } else {
            float w_frac = (float)(W_limit - currW) / items[i].weight;
            float v_frac = (float)(V_limit - currV) / items[i].volume;
            float frac = std::min(w_frac, v_frac);

            bound += items[i].value * frac;
            break;
            // Para aqui
            // Isso nao é uma solucao valida, é so um limite superior otimista
            // Se nem esse valor otimista supera a melhor solucao atual -> abandona o nó
        }
    }

    return bound;
}

Solution BranchAndBoundSolver::solve() {
    Solution sol;
    sol.maxValue = 0;
    sol.selectedItems.assign(n, 0);

    auto start = std::chrono::high_resolution_clock::now();

    // Sempre explora primeiro o nó com maior bound, onde parece mais promissor
    std::priority_queue<BBNode, std::vector<BBNode>, CompareBound> pq;

    // Nenhuma decisao tomada ainda
    BBNode root;
    root.level = -1;
    root.profit = 0;
    root.weight = 0;
    root.volume = 0;
    root.selection.assign(n, 0);
    root.bound = calculateBound(root);

    pq.push(root);

    // Enquanto ainda existem caminhos promissores
    while (!pq.empty()) {
        // Pega o mrlhor nó disponivel 
        // Esse é o caminho que parece mais promissor agr
        BBNode u = pq.top();
        pq.pop();

        // Teste de poda  - Mesmo no melhor caso, esse caminho nao ganha do que ja tem, joga fora
        if (u.bound <= sol.maxValue)
            continue;

        BBNode v;
        v.level = u.level + 1;

        if (v.level >= n)
            continue;

        // Ramificacao (Branch)
        // Duas opcoes para o proximo item:    
        // ---- PEGAR ITEM ----
        v.weight = u.weight + items[v.level].weight;
        v.volume = u.volume + items[v.level].volume;
        v.profit = u.profit + items[v.level].value;
        v.selection = u.selection;
        v.selection[v.level] = 1;

        // Se estourar limite -> ignora
        if (v.weight <= W_limit && v.volume <= V_limit) {
            // Se for melhor que o atual -> salva como melhor solucao
            if (v.profit > sol.maxValue) {
                sol.maxValue = v.profit;
                sol.selectedItems = v.selection;
            }

            v.bound = calculateBound(v);
            // Se o bound ainda for promissor -> entra na fila
            if (v.bound > sol.maxValue)
                pq.push(v);
        }

        // ---- NÃO PEGAR ITEM ----
        v.weight = u.weight;
        v.volume = u.volume;
        v.profit = u.profit;
        v.selection = u.selection;
        v.selection[v.level] = 0;
        // Calcula o bound
        v.bound = calculateBound(v);
        
        // Se ainda puder bater a melhor solucao -> entra na fila
        if (v.bound > sol.maxValue)
            pq.push(v);
    }

    // Isso se repete ate nao sobrar nenhum no promissor ou todos os caminhos viaveis terem sido testados

    auto end = std::chrono::high_resolution_clock::now();
    sol.executionTime =
        std::chrono::duration<double>(end - start).count();

    return sol;
    // sol.maxValue -> melhor valor encontrado
    // sol.selectItems -> quais itens geraram esse valor
}

