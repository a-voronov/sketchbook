#include "distances.h"

Distances::Distances(const Cell& root)
    : root_(&root) {
        cells_[root_] = 0;
    }

int Distances::get(const Cell& cell) const {
    if (cells_.count(&cell) == 0) {
        return -1;
    }
    return cells_.at(&cell);
}

void Distances::set(const Cell& cell, int distance) {
    cells_.insert_or_assign(&cell, distance);
}

vector<const Cell*> Distances::cells() const {
    vector<const Cell*> result{};
    result.reserve(cells_.size());
    for (const auto& kv : cells_)
        result.push_back(kv.first);
    return result;
}
