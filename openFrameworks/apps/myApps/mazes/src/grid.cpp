#include "grid.h"
#include <string>

namespace {
string repeat(string_view s, size_t n) {
    string out;
    out.reserve(s.size() * n);
    for (size_t i = 0; i < n; ++i)
        out += s;
    return out;
}
}

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

// ASCII grid output
std::ostream& operator<<(std::ostream& os, const Grid& grid) {
    os << "+" << repeat("---+", grid.columns()) << "\n";

    const string body = "   ";
    const string corner = "+";

    for (int row = 0; row < grid.rows(); ++row) {
        string top = "|";
        string bottom = "+";

        for (int col = 0; col < grid.columns(); ++col) {
            const Cell* cell = grid.cell_at(row, col);

            const string east_boundary = cell->is_linked(cell->east) ? " " : "|";
            top += body + east_boundary;

            const string south_boundary = cell->is_linked(cell->south) ? "   " : "---";
            bottom += south_boundary + corner;
        }

        os << top << "\n";
        os << bottom << "\n";
    }
    return os;
}
