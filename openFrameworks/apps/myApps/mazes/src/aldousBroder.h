#pragma once

#include "grid.h"

struct AldousBroder {
    static void on(Grid& grid, std::mt19937& rng) {
        auto cell = grid.random_cell(rng);
        auto unvisited = grid.size() - 1;

        while (unvisited > 0) {
            auto neighbor = cell->random_neighbor(rng);

            if (neighbor->links().empty()) {
                cell->link(neighbor);
                --unvisited;
            }

            cell = neighbor;
        }
    }
};
