#pragma once

#include "grid.h"

struct HuntAndKill {
    static void on(Grid& grid, std::mt19937& rng) {
        auto current = grid.random_cell(rng);

        while (current) {
            auto current_neighbors = current->neighbors();
            vector<Cell*> unvisited_neighbors;
            copy_if(current_neighbors.begin(), current_neighbors.end(), back_inserter(unvisited_neighbors), ([](auto& n) {
                return n->links().empty();
            }));

            if (unvisited_neighbors.empty()) {
                current = nullptr;

                // we're using a single loop to control it through break and continue statements,
                // and here grid.size() for masked grid will be not what we're looking for, so we calculate it for each cell
                int grid_size = grid.rows() * grid.columns();
                for (int i = 0; i < grid_size; ++i) {
                    int r = i / grid.columns();
                    int c = i % grid.columns();
                    auto cell = grid.cell_at(r, c);
                    if (!cell) continue;

                    auto cell_neighbors = cell->neighbors();
                    vector<Cell*> visited_neighbors;
                    copy_if(cell_neighbors.begin(), cell_neighbors.end(), back_inserter(visited_neighbors), ([](auto& n) {
                        return !n->links().empty();
                    }));

                    if (cell->links().empty() && !visited_neighbors.empty()) {
                        std::uniform_int_distribution<int> vn_dist(0, visited_neighbors.size() - 1);
                        current = cell;
                        auto neighbor = visited_neighbors.at(vn_dist(rng));
                        current->link(neighbor);

                        break;
                    }
                }
            } else {
                std::uniform_int_distribution<int> un_dist(0, unvisited_neighbors.size() - 1);
                auto neighbor = unvisited_neighbors.at(un_dist(rng));
                current->link(neighbor);
                current = neighbor;
            }
        }
    }
};
