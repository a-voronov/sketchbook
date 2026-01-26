#include "cell.h"
#include "distances.h"

void Cell::link(Cell* other, bool bidi) {
    if (!other || other == this) return;

    if (std::find(links_.begin(), links_.end(), other) == links_.end())
        links_.push_back(other);

    if (bidi)
        other->link(this, false);
}

void Cell::unlink(Cell* other, bool bidi) {
    if (!other || other == this) return;

    auto it = std::find(links_.begin(), links_.end(), other);
    if (it != links_.end())
        links_.erase(it);

    if (bidi)
        other->unlink(this, false);
}

bool Cell::is_linked(const Cell* other) const {
    if (!other || other == this) return false;

    return find(links_.begin(), links_.end(), other) != links_.end();
}

vector<const Cell*> Cell::neighbors() const {
    vector<const Cell*> result;
    result.reserve(4);
    if (north != nullptr) result.push_back(north);
    if (south != nullptr) result.push_back(south);
    if (east  != nullptr) result.push_back(east);
    if (west  != nullptr) result.push_back(west);
    return result;
}

Distances Cell::distances() {
    Distances distances{*this};
    vector<const Cell*> frontier{this};

    while (!frontier.empty()) {
        vector<const Cell*> new_frontier{};

        for (auto cell : frontier) {
            for (auto linked : cell->links()) {
                if (distances.get(*linked) < 0) {
                    distances.set(*linked, distances.get(*cell) + 1);
                    new_frontier.emplace_back(linked);
                }
            }
        }

        frontier = new_frontier;
    }

    return distances;
}
