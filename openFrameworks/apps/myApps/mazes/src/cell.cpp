#include "cell.h"

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
    // auto ref_other = *other;
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
