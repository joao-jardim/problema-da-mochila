#ifndef BRANCH_AND_BOUND_SOLVER_H
#define BRANCH_AND_BOUND_SOLVER_H

#include "solver.h"
#include <vector>

class BranchAndBoundSolver : public KnapsackSolver {
public:
    BranchAndBoundSolver(int W, int V, const std::vector<Item> &items);
    Solution solve() override;

private:
    struct BBNode {
        int level;
        int profit;
        int weight;
        int volume;
        float bound;
        std::vector<int> selection;
    };

    struct CompareBound {
        bool operator()(const BBNode &a, const BBNode &b) const {
            return a.bound < b.bound; // max-heap
        }
    };

    float calculateBound(const BBNode &u) const;
};

#endif


