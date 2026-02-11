#pragma once

#include <memory>
#include <vector>
#include <random>

using namespace std;

struct Mask {
public:
    Mask(int r, int c);

    static unique_ptr<Mask> from_txt(string file_path);
    static unique_ptr<Mask> from_png(string file_path);

    int rows() const { return rows_; }
    int columns() const { return columns_; }

    bool bit_at(int r, int c) const;
    void set_bit_at(int r, int c, bool bit);

    int count() const;
    pair<int, int> random_location(std::mt19937& rng) const;

private:
    int rows_;
    int columns_;
    vector<vector<bool>> bits_;
};
