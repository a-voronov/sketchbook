#pragma once

#include "cell.h"
#include <random>

using namespace std;

struct Grid {
public:
    Grid(int r, int c);

    template<class F>
    void each_cell(F&& f);

    int size() const;

private:
    int rows_, columns_;
    // Grid owns cells, so we store them as unique_ptr and pass around raw pointers for temporary access to cells
    vector<vector<unique_ptr<Cell>>> grid_;

    const Cell* cell_at(int row, int column) const;
    const Cell& random_cell(std::mt19937& rng) const;

    void prepare_grid();
    void configure_cells();
};
