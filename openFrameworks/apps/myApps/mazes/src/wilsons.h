#pragma once

#include <random>

struct Grid;

struct Wilsons {
    static void on(Grid& grid, std::mt19937& rng) {
        vector<Cell*> unvisited;
        grid.each_cell([&unvisited](Cell& cell) {
            unvisited.push_back(&cell);
        });

        std::uniform_int_distribution<int> first_dist(0, unvisited.size() - 1);
        auto first_idx = first_dist(rng);
        unvisited.erase(unvisited.begin() + first_idx);

        while (!unvisited.empty()) {
            vector<Cell*> path;
            std::uniform_int_distribution<int> unvisited_dist(0, unvisited.size() - 1);
            auto cell = unvisited.at(unvisited_dist(rng));
            path.push_back(cell);

            while(find(unvisited.begin(), unvisited.end(), cell) != unvisited.end()) {
                cell = cell->random_neighbor(rng);
                auto position = find(path.begin(), path.end(), cell);

                if (position != path.end()) {
                    path = vector<Cell*>(path.begin(), position + 1);
                } else {
                    path.emplace_back(cell);
                }
            }

            for (int idx = 0; idx < path.size() - 1; ++idx) {
                path.at(idx)->link(path.at(idx + 1));

                auto erase_idx = find(unvisited.begin(), unvisited.end(), path.at(idx));
                if (erase_idx != unvisited.end())
                    unvisited.erase(erase_idx);
            }
        }
    }
};
