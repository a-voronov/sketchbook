#pragma once

#include "distances.h"
#include "grid.h"
#include "mazeUtils.h"

using namespace std;

struct ColorGrid : Grid {
public:
    ColorGrid(int rows, int columns)
        : Grid(rows, columns) {}
    ColorGrid()
        : ColorGrid(0, 0) {}

    void set_distances(const Distances& distances) {
        distances_ = make_unique<Distances>(distances);
        maximum_   = distances.max().second;
    }

    ofColor bg_color_for(const Cell& cell, const DrawCfg& draw_cfg) const override {
        if (!distances_) return ofColor::white;
        int distance = distances_->get(cell);
        if (distance < 0) return ofColor::white;

        float intensity = 1.0f - ((maximum_ - distance) / maximum_);

        return color_intensity(draw_cfg.tone, intensity * std::clamp(draw_cfg.intensity_stretch, 0.0f, 1.0f));
    }

private:
    unique_ptr<Distances> distances_ = nullptr;
    float maximum_;
};
