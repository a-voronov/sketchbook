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

    void accept(GridVisitor& visitor, std::mt19937& rng) override { visitor.visit(*this, rng); }

private:
    unique_ptr<Distances> distances_ = nullptr;
};
