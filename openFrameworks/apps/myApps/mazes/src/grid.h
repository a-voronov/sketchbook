#pragma once

#include <string>
#include <memory>
#include <functional>
#include <random>
#include <ostream>

#include "cell.h"
#include "ofMain.h"

using namespace std;

struct ColorCfg {
    const ofColor tone;
    // 0...1
    const float intensity_stretch;

    ColorCfg(ofColor t = ofColor::white, float s = 0.75f) : tone(t), intensity_stretch(s) {};
};

struct Grid {
public:
    Grid(int rows, int columns);
    Grid() : Grid(0, 0) {};

    int rows() const    { return rows_; }
    int columns() const { return columns_; }
    int size() const    { return rows_ * columns_; }

    void each_cell(const std::function<void(Cell&)>& lambda);
    Cell* cell_at(int row, int column) const;
    Cell& random_cell(std::mt19937& rng) const;

    void draw(int cell_size = 10, const ColorCfg& color_cfg = {});
    virtual string contents_of(const Cell& cell) const;
    virtual ofColor background_color_for(const Cell& cell, const ColorCfg& color_cfg) const { return color_cfg.tone; }

private:
    int rows_, columns_;
    // Grid owns cells, so we store them as unique_ptr
    // and pass around raw pointers for temporary access to the cells
    vector<vector<unique_ptr<Cell>>> grid_;

    void prepare_grid();
    void configure_cells();
};

std::ostream& operator<<(std::ostream& os, const Grid& grid);
