#include "sidewinder.h"

void Sidewinder::on(Grid& grid, std::mt19937& rng) {
    vector<Cell*>run;
    run.reserve(grid.columns());
    for (int row = 0; row < grid.rows(); ++row) {
        run.clear();

        for (int col = 0; col < grid.columns(); ++col) {
            Cell* cell = grid.cell_at(row, col);
            run.push_back(cell);

            bool at_eastern_boundary  = cell->east == nullptr;
            bool at_northern_boundary = cell->north == nullptr;

            std::uniform_int_distribution<int> boundary_dist(0, 1);
            bool should_close_out = at_eastern_boundary
                || (!at_northern_boundary && boundary_dist(rng) == 0);

            if (should_close_out) {
                std::uniform_int_distribution<int> member_dist(0, run.size() - 1);
                Cell* member = run.at(member_dist(rng));
                if (member->north != nullptr) {
                    member->link(member->north);
                }
                run.clear();
            } else {
                cell->link(cell->east);
            }
        }
    }
}
