#include "grid.h"

Grid::Grid(int r, int c)
    : rows_(r), columns_(c) {
        prepare_grid();
        configure_cells();
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
    each_cell([this](Cell &cell) {
        int row = cell.row;
        int column = cell.column;

        cell.north = cell_at(row - 1, column);
        cell.south = cell_at(row + 1, column);
        cell.west  = cell_at(row, column - 1);
        cell.east  = cell_at(row, column + 1);
    });
}

template<class F>
void Grid::each_cell(F&& f) {
    for (auto& row : grid_)
        for (auto& cell : row)
            f(*cell);
}

int Grid::size() const {
    return rows_ * columns_;
}

const Cell* Grid::cell_at(int row, int column) const {
    if (row >= grid_.size()) return nullptr;
    if (column >= grid_.at(row).size()) return nullptr;
    return grid_.at(row).at(column).get();
}

const Cell& Grid::random_cell(std::mt19937& rng) const {
    std::uniform_int_distribution<int> r_dist(0, rows_ - 1);
    std::uniform_int_distribution<int> c_dist(0, columns_ - 1);
    return *grid_.at(r_dist(rng)).at(c_dist(rng));
}
