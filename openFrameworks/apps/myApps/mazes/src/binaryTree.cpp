#include "binaryTree.h"

void BinaryTree::on(Grid& grid, std::mt19937& rng) {
    grid.each_cell([&rng](Cell& cell) {
        vector<Cell*> neighbors;

        if (cell.north != nullptr) neighbors.push_back(cell.north);
        if (cell.east  != nullptr) neighbors.push_back(cell.east);

        if (neighbors.empty()) return;

        std::uniform_int_distribution<int> dist(0, neighbors.size() - 1);
        int idx = dist(rng);
        auto neighbor = neighbors.at(idx);

        if (neighbor != nullptr) cell.link(neighbor);
    });
}
