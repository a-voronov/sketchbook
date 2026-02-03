#pragma once

#include <random>

struct Grid;

struct BinaryTree {
    static void on(Grid& grid, std::mt19937& rng) {
        grid.each_cell([&rng](Cell& cell) {
            std::uniform_int_distribution<int> dist(0, 1);

            Cell *neighbor = dist(rng) == 0
                ? (cell.north ?: cell.east)
                : (cell.east ?: cell.north);

            cell.link(neighbor);
        });
    }
};
