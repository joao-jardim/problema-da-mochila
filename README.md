# Knapsack 0-1 with Two Constraints (Weight & Volume)

Trabalho Prático de Projeto e Análise de Algoritmos (PAA).
Implementação e análise comparativa de três abordagens para o problema da Mochila 0-1 com restrições de Peso (W) e Volume (V).

## Algoritmos Implementados

1.  **Backtracking (`BT`)**: Busca exaustiva com poda por viabilidade.
2.  **Branch and Bound (`BB`)**: Busca em largura (BFS) utilizando limitantes superiores (Upper Bound) para podar ramos não promissores.
3.  **Programação Dinâmica (`DP`)**: Abordagem exata utilizando uma tabela de memorização 3D `dp[item][peso][volume]`.

## Requisitos

- Compilador C++17 (g++)
- Python 3.x (para scripts de teste e análise)
- Bibliotecas Python: `pandas`, `matplotlib`, `seaborn`, `scipy`

## Compilação

Para compilar o código C++, utilize o `make`:

```bash
make
```

Isso gerará o executável `./solver`.

## Execução Manual

O programa aceita como entrada um arquivo de texto e a estratégia desejada:

```bash
./solver <ESTRATEGIA> <ARQUIVO_ENTRADA>
```

- **ESTRATEGIA**: `BT`, `BB`, ou `DP`
- **ARQUIVO_ENTRADA**: Caminho para o arquivo contendo a instância.

**Formato da Entrada**:
```text
W V
w1 l1 v1
w2 l2 v2
...
```

**Exemplo**:
```bash
./solver DP test_input.txt
```

## Benchmarks e Análise Estatística

Para reproduzir os experimentos, gerar instâncias aleatórias e gerar os gráficos:

```bash
python3 scripts/benchmark.py
```

O script irá:
1.  Gerar 10 instâncias para diferentes tamanhos de entrada (N=4, 8, 12, ...).
2.  Executar os 3 algoritmos.
3.  Salvar os resultados em `results.csv`.
4.  Gerar gráficos em `output/time_comparison.png`.
5.  Executar o teste estatístico de Friedman para verificar diferenças significativas.

## Resultados Obtidos

Os testes demonstraram que o algoritmo de Backtracking possui crescimento exponencial acentuado. O Branch and Bound oferece melhorias, mas depende da qualidade dos limitantes. A Programação Dinâmica mostrou-se mais estável para os tamanhos testados, embora seu consumo de memória cresça com W e V.

O teste de Friedman confirmou diferença estatística significativa entre os algoritmos para todas as configurações de tamanho testadas (p-value < 0.05).
