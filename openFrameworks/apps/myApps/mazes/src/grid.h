#pragma once

#include <string>
#include <memory>
#include <functional>
#include <random>
#include <ostream>

#include "cell.h"
#include "ofMain.h"

using namespace std;

enum struct DrawMode {
    Walls, BgColor, All
};

struct DrawCfg {
    ofColor wall = ofColor::black;
    ofColor tone = ofColor::white;
    // 0...1
    float intensity_stretch = 0.75f;
    int cell_size = 10;
    int wall_width = 2;
    DrawMode draw_mode = DrawMode::All;
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

    void draw(const DrawCfg& draw_cfg = {}) const;
    void draw_cells(vector<const Cell*> cells, const DrawCfg& draw_cfg) const;
    virtual string contents_of(const Cell& cell) const;
    virtual ofColor bg_color_for(const Cell& cell, const DrawCfg& draw_cfg) const { return draw_cfg.tone; }

private:
    int rows_, columns_;
    // Grid owns cells, so we store them as unique_ptr
    // and pass around raw pointers for temporary access to the cells
    vector<vector<unique_ptr<Cell>>> grid_;

    void prepare_grid();
    void configure_cells();

    void draw_cell(const Cell& cell, const DrawCfg& draw_cfg) const;
    void draw_bg_color(ofPoint p1, const Cell& cell, const DrawCfg& draw_cfg) const;
    void draw_walls(ofPoint p1, ofPoint p2, const Cell& cell, const DrawCfg& draw_cfg) const;
};

std::ostream& operator<<(std::ostream& os, const Grid& grid);
