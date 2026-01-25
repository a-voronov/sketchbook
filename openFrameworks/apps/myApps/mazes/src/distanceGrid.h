#pragma once

#include "grid.h"
#include "distances.h"
#include "mazeUtils.h"

using namespace std;

struct DistanceGrid: Grid {
public:
    unique_ptr<Distances> distances = nullptr;

    DistanceGrid(int rows, int columns) : Grid(rows, columns) {}
    DistanceGrid() : DistanceGrid(0, 0) {}

    string contents_of(const Cell& cell) const override {
        if (distances) {
            int distance = distances->get(cell);
            if (distance != -1) {
                return to_base36(distance);
            }
        }
        return Grid::contents_of(cell);
    }
};
