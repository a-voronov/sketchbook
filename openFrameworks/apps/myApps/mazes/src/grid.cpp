#include "grid.h"

Grid::Grid(int rows, int columns)
    : rows_(rows), columns_(columns) {
        prepare_grid();
        configure_cells();
    }

void Grid::each_cell(const std::function<void(Cell&)>& lambda) {
    for (auto& row : grid_)
        for (auto& cell : row)
            lambda(*cell);
}

Cell* Grid::cell_at(int row, int column) const {
    if (row >= grid_.size()) return nullptr;
    if (column >= grid_.at(row).size()) return nullptr;
    return grid_.at(row).at(column).get();
}

Cell& Grid::random_cell(std::mt19937& rng) const {
    std::uniform_int_distribution<int> r_dist(0, rows_ - 1);
    std::uniform_int_distribution<int> c_dist(0, columns_ - 1);
    return *grid_.at(r_dist(rng)).at(c_dist(rng));
}

void Grid::prepare_grid() {
    grid_.clear();
    grid_.reserve(rows_);
    for (int r = 0; r < rows_; ++r) {
        // constructing empty row directly in the grid and then filling it,
        // so we can avoid copying/moving it around
        grid_.emplace_back();
        auto& row = grid_.back();
        row.reserve(columns_);
        for (int c = 0; c < columns_; ++c) {
            row.emplace_back(make_unique<Cell>(r, c));
        }
    }
}

void Grid::configure_cells() {
    each_cell([this](Cell& cell) {
        int row = cell.row();
        int column = cell.column();

        cell.north = cell_at(row - 1, column);
        cell.south = cell_at(row + 1, column);
        cell.west  = cell_at(row, column - 1);
        cell.east  = cell_at(row, column + 1);
    });
}
