#pragma once

#include "grid.h"
#include "distances.h"
#include "mazeUtils.h"

using namespace std;

struct DistanceGrid : Grid {
public:
    DistanceGrid(int rows, int columns)
        : Grid(rows, columns) {}
    DistanceGrid()
        : DistanceGrid(0, 0) {}

    void set_distances(const Distances& distances) {
        distances_ = make_unique<Distances>(distances);
        max_ = distances.max().first;
    }

    string contents_of(const Cell& cell) const override {
        if (distances_) {
            int distance = distances_->get(cell);
            if (distance >= 0) {
                return to_base36(distance);
            }
        }
        return Grid::contents_of(cell);
    }

    ofColor bg_color_for(const Cell& cell, const DrawCfg& draw_cfg) const override {
        if (!max_ || !distances_)
            return ofColor::white;

        if (cell == distances_.get()->get_root()) {
            return ofColor::blue;
        } else if (cell == *max_) {
            return ofColor::red;
        }
        return draw_cfg.tone;
    }

    void accept(GridVisitor& visitor, std::mt19937& rng) override { visitor.visit(*this, rng); }

private:
    unique_ptr<Distances> distances_ = nullptr;
    const Cell* max_;
};
