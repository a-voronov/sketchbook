#pragma once

#include "grid.h"
#include "mask.h"

using namespace std;

struct MaskedGrid : Grid {
public:
    explicit MaskedGrid(unique_ptr<Mask> mask)
        : Grid(mask->rows(), mask->columns()), mask_(std::move(mask)) {
            prepare_grid();
            configure_cells();
        }

    MaskedGrid()
        : MaskedGrid(make_unique<Mask>(0, 0)) {}

    void prepare_grid() override {
        grid_.clear();
        grid_.reserve(rows_);
        for (int r = 0; r < rows_; ++r) {
            // constructing empty row directly in the grid and then filling it,
            // so we can avoid copying/moving it around
            grid_.emplace_back();
            auto& row = grid_.back();
            row.reserve(columns_);
            for (int c = 0; c < columns_; ++c) {
                if (mask_->bit_at(r, c)) {
                    row.emplace_back(make_unique<Cell>(r, c));
                } else {
                    row.emplace_back(nullptr);
                }
            }
        }
    }

    Cell* random_cell(std::mt19937& rng) const override {
        auto [r, c] = mask_->random_location(rng);
        return cell_at(r, c);
    }

    int size() const override {
        return mask_->count();
    }

    void accept(GridVisitor& visitor, std::mt19937& rng) override { visitor.visit(*this, rng); }

private:
    unique_ptr<Mask> mask_;
};
