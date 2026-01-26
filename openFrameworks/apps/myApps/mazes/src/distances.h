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

    // returning Distances as a value since it's a computed data which has no prior owner,
    // so the caller owns it and can do whatever they want with it
    Distances path_to(const Cell& goal) const;

private:
    // keeping cells as const* because we don't need to mutate them here
    const Cell* root_;
    unordered_map<const Cell*, int> cells_{};
};
