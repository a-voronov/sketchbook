#pragma once

#include<unordered_map>

#include "cell.h"

using namespace std;

struct Distances {
public:
    Distances(const Cell& root);

    int get(const Cell& cell) const;
    void set(const Cell& cell, int distance);

    vector<const Cell*> cells() const;

private:
    // keeping cells as const* because we don't need to mutate them here
    const Cell* root_;
    unordered_map<const Cell*, int> cells_{};
};
