#pragma once

#include <random>

// struct Grid;
#include "grid.h"

struct HuntAndKill {
    static void on(Grid& grid, std::mt19937& rng) {
        auto current = grid.random_cell(rng);

        while (current) {
            auto cell_neighbors = current->neighbors();
            vector<Cell*> unvisited_neighbors;
            copy_if(cell_neighbors.begin(), cell_neighbors.end(), back_inserter(unvisited_neighbors), ([](auto& n) {
                return n->links().empty();
            }));

            if (unvisited_neighbors.empty()) {
                current = nullptr;

                for (int i = 0; i < grid.size(); ++i) {
                    int r = i / grid.columns();
                    int c = i % grid.columns();
                    auto& cell = *grid.cell_at(r, c);

                    auto cell_neighbors = cell.neighbors();
                    vector<Cell*> visited_neighbors;
                    copy_if(cell_neighbors.begin(), cell_neighbors.end(), back_inserter(visited_neighbors), ([](auto& n) {
                        return !n->links().empty();
                    }));

                    if (cell.links().empty() && !visited_neighbors.empty()) {
                        std::uniform_int_distribution<int> vn_dist(0, visited_neighbors.size() - 1);
                        current = &cell;
                        auto neighbor = visited_neighbors.at(vn_dist(rng));
                        current->link(neighbor);

                        break;
                    }
                }

                // grid.each_cell([&](auto& cell){
                //     auto cell_neighbors = cell.neighbors();
                //     vector<Cell*> visited_neighbors;
                //     copy_if(cell_neighbors.begin(), cell_neighbors.end(), back_inserter(visited_neighbors), ([](auto& n) {
                //         return !n.links().empty();
                //     }));

                //     if (cell.links().empty() && !visited_neighbors.empty()) {
                //         std::uniform_int_distribution<int> vn_dist(0, visited_neighbors.size() - 1);
                //         current = &cell;
                //         auto neighbor = visited_neighbors.at(vn_dist(rng));
                //         current->link(neighbor);

                //         // break; TODO: maybe each_cell lambda is not good for this
                //     }
                // });

            } else {
                std::uniform_int_distribution<int> un_dist(0, unvisited_neighbors.size() - 1);
                auto neighbor = unvisited_neighbors.at(un_dist(rng));
                current->link(neighbor);
                current = neighbor;
            }
        }
    }
};

// class HuntAndKill
//   def self.on(grid)
//     current = grid.random_cell

//     while current
//       unvisited_neighbors = current.neighbors.select { |n| n.links.empty? }

//       if unvisited_neighbors.any?
//         neighbor = unvisited_neighbors.sample
//         current.link(neighbor)
//         current = neighbor
//       else
//         current = nil

//         grid.each_cell do |cell|
//           visited_neighbors = cell.neighbors.select { |n| n.links.any? }
//           if cell.links.empty? && visited_neighbors.any?
//             current = cell

//             neighbor = visited_neighbors.sample
//             current.link(neighbor)

//             break
//           end
//         end
//       end
//     end
//     grid
//   end
// end