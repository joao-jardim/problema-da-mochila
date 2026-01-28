# Relatório Técnico: Implementação do Backtracking para o Problema da Mochila

## 1. Introdução e Teoria
O **Backtracking** é uma estratégia algorítmica para encontrar soluções para problemas de otimização combinatória de forma exaustiva. Ele modela o espaço de soluções como uma árvore, onde cada nó representa uma decisão parcial. Ao contrário da força bruta ingênua, o backtracking evita explorar ramos que violam as restrições do problema (processo conhecido como *pruning* ou poda).

Neste contexto, o Problema da Mochila Multidimensional (com restrições de Peso $W$ e Volume $V$) foi abordado utilizando um algoritmo de backtracking genérico.

## 2. Detalhes da Implementação
A implementação em C++ (`src/backtracking.cpp`) segue estritamente o modelo de projeto genérico. A classe `BacktrackingSolver` utiliza a função recursiva `backtrack` e `construct_candidates` para explorar a árvore com poda de viabilidade.

## 3. Caracterização dos Testes (Dataset)
Para validar a implementação e analisar o desempenho, foram geradas instâncias de teste sintéticas com as seguintes características:

*   **Tamanhos ($N$)**: Variando de 4 a 24 itens, com passo de 4 ($N \in \{4, 8, 12, 16, 20, 24\}$).
*   **Capacidades ($W, V$)**: Definidas proporcionalmente a $N$ para manter a "densidade" do problema constante:
    *   $W = 20 \times N$
    *   $V = 20 \times N$
*   **Itens**: Gerados pseudo-aleatoriamente (`scripts/generator.py`):
    *   **Pesos ($w$) e Volumes ($l$)**: Inteiros aleatórios entre 1 e 50 (uniforme).
    *   **Valores ($v$)**: Inteiros aleatórios entre 1 e 100 (uniforme).
*   **Amostragem**: Para cada tamanho $N$, foram geradas e testadas **30 instâncias independentes** para garantir significância estatística e mitigar o efeito de casos isolados de "sorte" ou "azar" (especialmente relevante para o Branch & Bound).

## 4. Análise Assintótica (Teórica)

| Métrica | Complexidade | Explicação |
| :--- | :--- | :--- |
| **Tempo** | $O(2^n)$ | No pior caso, visita todos os nós. Recorrência $T(n) = 2T(n-1) + O(1)$. |
| **Espaço** | $O(n)$ | Memória proporcional à pilha de recursão. |

## 5. Análise Comparativa e Experimental

### 5.1 Backtracking (BT) vs Branch & Bound (BB) e DP
*   **BT**: Sofreu crescimento exponencial. De $N=20 \to 24$, o tempo saltou de ~0.09s para ~1.58s (+1700%), confirmanda a inviabilidade para $N > 40$.
*   **BB**: Manteve-se na ordem de microsegundos, mostrando que a poda por otimalidade é crucial para este tipo de problema.
*   **DP**: Manteve desempenho estável e previsível, dependendo das capacidades lineares.

### 5.2 Conclusão
O gráfico de comparação de tempo evidenciou a natureza exponencial do Backtracking. Embora correto e eficiente em memória ($O(n)$), o BT puro é inadequado para o cenário de teste gerado ($N$ crescente), sendo rapidamente superado por estratégias mais inteligentes (BB) ou estruturadas (DP).
