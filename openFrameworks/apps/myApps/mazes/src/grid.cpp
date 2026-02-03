#include "grid.h"
#include "ofMain.h"
#include "mazeUtils.h"

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

Cell* Grid::random_cell(std::mt19937& rng) const {
    std::uniform_int_distribution<int> r_dist(0, rows_ - 1);
    std::uniform_int_distribution<int> c_dist(0, columns_ - 1);
    return grid_.at(r_dist(rng)).at(c_dist(rng)).get();
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
        for (int c = 0; c < columns_; ++c)
            row.emplace_back(make_unique<Cell>(r, c));
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

void Grid::draw(const DrawCfg& draw_cfg) const {
    ofPushMatrix();
        ofTranslate(
            ofGetWidth() / 2 - (draw_cfg.cell_size * columns_) / 2,
            ofGetHeight() / 2 - (draw_cfg.cell_size * rows_) / 2
        );

        for (int row = 0; row < rows_; ++row)
            for (int col = 0; col < columns_; ++col)
                draw_cell(*cell_at(row, col), draw_cfg);

    ofPopMatrix();
}

void Grid::draw_cells(const vector<const Cell*>& cells, const DrawCfg& draw_cfg) const {
    ofPushMatrix();
        ofTranslate(
            ofGetWidth() / 2 - (draw_cfg.cell_size * columns_) / 2,
            ofGetHeight() / 2 - (draw_cfg.cell_size * rows_) / 2
        );

        for (auto cell : cells)
            draw_cell(*cell_at(cell->row(), cell->column()), draw_cfg);

    ofPopMatrix();
}

void Grid::draw_cell(const Cell& cell, const DrawCfg& draw_cfg) const {
    ofPoint p1{float(cell.column() * draw_cfg.cell_size), float(cell.row() * draw_cfg.cell_size)};
    ofPoint p2{float((cell.column() + 1) * draw_cfg.cell_size), float((cell.row() + 1) * draw_cfg.cell_size)};

    switch (draw_cfg.draw_mode) {
    case DrawMode::Walls:
        draw_walls(p1, p2, cell, draw_cfg);
        break;
    case DrawMode::BgColor:
        draw_bg_color(p1, cell, draw_cfg);
        break;
    case DrawMode::All:
        draw_walls(p1, p2, cell, draw_cfg);
        draw_bg_color(p1, cell, draw_cfg);
        break;
    }
}

void Grid::draw_bg_color(ofPoint p1, const Cell& cell, const DrawCfg& draw_cfg) const {
    ofFill();
    ofSetColor(bg_color_for(cell, draw_cfg));

    float wall_padding = float(draw_cfg.wall_width) / 2.0f;
    float north{}, south{}, east{}, west{};

    if (!cell.is_linked(cell.north)) north = wall_padding;
    if (!cell.is_linked(cell.south)) south = wall_padding;
    if (!cell.is_linked(cell.east))  east = wall_padding;
    if (!cell.is_linked(cell.west))  west = wall_padding;

    ofDrawRectangle(p1.x + west, p1.y + north, draw_cfg.cell_size - east - west, draw_cfg.cell_size - south - north);
}

void Grid::draw_walls(ofPoint p1, ofPoint p2, const Cell& cell, const DrawCfg& draw_cfg) const {
    ofSetLineWidth(draw_cfg.wall_width);
    ofNoFill();
    ofSetColor(draw_cfg.wall);
    if (cell.north == nullptr) ofDrawLine(p1.x, p1.y, p2.x, p1.y);
    if (cell.west == nullptr) ofDrawLine(p1.x, p1.y, p1.x, p2.y);

    if (!cell.is_linked(cell.east)) ofDrawLine(p2.x, p1.y, p2.x, p2.y);
    if (!cell.is_linked(cell.south)) ofDrawLine(p1.x, p2.y, p2.x, p2.y);
}

string Grid::contents_of(const Cell& cell) const {
    return " ";
}

// ASCII grid output
std::ostream& operator<<(std::ostream& os, const Grid& grid) {
    os << "+" << repeat("---+", grid.columns()) << "\n";

    const string corner = "+";

    for (int row = 0; row < grid.rows(); ++row) {
        string top = "|";
        string bottom = "+";

        for (int col = 0; col < grid.columns(); ++col) {
            const Cell* cell = grid.cell_at(row, col);
            const string body = " " + grid.contents_of(*cell) + " ";

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
