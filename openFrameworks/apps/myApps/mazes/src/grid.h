#pragma once

#include "cell.h"
#include <functional>
#include <random>
#include <ostream>

using namespace std;

struct Grid {
public:
    Grid(int rows, int columns);

    int rows() const    { return rows_; }
    int columns() const { return columns_; }
    int size() const    { return rows_ * columns_; }

    void each_cell(const std::function<void(Cell&)>& lambda);
    Cell* cell_at(int row, int column) const;
    Cell& random_cell(std::mt19937& rng) const;

private:
    int rows_, columns_;
    // Grid owns cells, so we store them as unique_ptr
    // and pass around raw pointers for temporary access to the cells
    vector<vector<unique_ptr<Cell>>> grid_;

    void prepare_grid();
    void configure_cells();
};

std::ostream& operator<<(std::ostream& os, const Grid& grid);
