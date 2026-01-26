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
    for (auto& kv : cells_)
        result.push_back(kv.first);
    return result;
}

Distances Distances::path_to(const Cell& goal) const {
    const Cell* current = &goal;

    Distances breadcrumbs{*root_};
    breadcrumbs.set(*current, get(*current));

    while (current != root_) {
        for (auto neighbor : current->links()) {
            if (get(*neighbor) < get(*current)) {
                breadcrumbs.set(*neighbor, get(*neighbor));
                current = neighbor;
                break;
            }
        }
    }
    return breadcrumbs;
}

pair<const Cell*, int> Distances::max() const {
    int max_distance = 0;
    const Cell* max_cell = root_;

    for (auto& [cell, distance] : cells_) {
        if (distance > max_distance) {
            max_cell = cell;
            max_distance = distance;
        }
    }
    return {max_cell, max_distance};
}
