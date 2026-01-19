#pragma once

#include <memory>
#include <vector>

using namespace std;

// Cell doesn't own other cells, and all cells exist within a common Grid,
// hence raw pointers are used here to refer to neighboring and linked cells
struct Cell {
public:
    Cell* north = nullptr;
    Cell* south = nullptr;
    Cell* east  = nullptr;
    Cell* west  = nullptr;

    Cell(int row, int column) :
        row_(row), column_(column) {}

    int row() const    { return row_; }
    int column() const { return column_; }

    void link(Cell* other, bool bidi = true);
    void unlink(Cell* other, bool bidi = true);
    bool is_linked(const Cell& other) const;

    // returning const& since this method serves as a read-only-view of underlying links_ member property,
    // so we don't make a copy and don't allow mutation of it
    const vector<const Cell*>& links() const { return links_; }
    // returning vector as a value since it's a computed data which has no prior owner,
    // so the caller owns it and can do whatever they want with it
    vector<const Cell*> neighbors() const;

private:
    int row_, column_;

    // there won't be too many interlinked cells, so vector should be enough vs heavier sets;
    // keeping cells as const* because we don't need to mutate linked cells
    vector<const Cell*> links_;
};
