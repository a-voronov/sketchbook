#pragma once

#include "grid.h"

struct RecursiveBacktracker {
    static void on(Grid& grid, std::mt19937& rng) {
        std::stack<Cell*> stack;
        auto start = grid.random_cell(rng);
        stack.push(start);

        while (!stack.empty()) {
            auto current = stack.top();
            auto current_neighbors = current->neighbors();

            vector<Cell*> neighbors;
            copy_if(current_neighbors.begin(), current_neighbors.end(), back_inserter(neighbors), ([](auto& n) {
                return n->links().empty();
            }));

            if (neighbors.empty()) {
                stack.pop();
            } else {
                std::uniform_int_distribution<int> n_dist(0, neighbors.size() - 1);
                auto neighbor = neighbors.at(n_dist(rng));
                current->link(neighbor);
                stack.push(neighbor);
            }
        }
    }
};
