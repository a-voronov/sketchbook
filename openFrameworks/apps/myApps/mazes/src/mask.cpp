#include "mask.h"
#include "ofImage.h"

#include <fstream>

Mask::Mask(int r, int c) : rows_(r), columns_(c) {
    bits_.reserve(rows_);
    for (int r = 0; r < rows_; ++r) {
        bits_.emplace_back();
        auto& row = bits_.back();
        row.reserve(columns_);
        for (int c = 0; c < columns_; ++c)
            row.emplace_back(true);
    }
}

unique_ptr<Mask> Mask::from_txt(string file_path) {
    ifstream file(file_path);
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            lines.push_back(line);
    }
    file.close();

    if (lines.empty())
        make_unique<Mask>(0, 0);

    int rows = lines.size();
    int columns = lines.at(0).size();
    auto mask = make_unique<Mask>(rows, columns);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            bool is_on = lines.at(r).at(c) != 'X';
            mask->set_bit_at(r, c, is_on);
        }
    }
    return mask;
}

unique_ptr<Mask> Mask::from_png(string file_path) {
    ofImage image;
    if (!image.load(file_path))
        return nullptr;

    auto mask = make_unique<Mask>(image.getHeight(), image.getWidth());
    auto pixels = image.getPixels();

    for (int r = 0; r < mask->rows(); ++r) {
        for (int c = 0; c < mask->columns(); ++c) {
            auto color = pixels.getColor(pixels.getPixelIndex(c, r));
            bool is_on = color != ofColor::black;
            mask->set_bit_at(r, c, is_on);
        }
    }
    return mask;
}

bool Mask::bit_at(int r, int c) const {
    if (r < 0 || r >= rows_) return false;
    if (c < 0 || c >= bits_.at(r).size()) return false;
    return bits_.at(r).at(c);
}

void Mask::set_bit_at(int r, int c, bool bit) {
    if (r < 0 || r >= rows_) return;
    if (c < 0 || c >= bits_.at(r).size()) return;
    bits_.at(r).at(c) = bit;
}

int Mask::count() const {
    int count = 0;
    for (int r = 0; r < rows_; ++r)
        for (int c = 0; c < columns_; ++c)
            if (bits_.at(r).at(c))
                ++count;

    return count;
}

pair<int, int> Mask::random_location(std::mt19937& rng) const {
    while (true) {
        std::uniform_int_distribution<int> r_dist(0, rows_ - 1);
        std::uniform_int_distribution<int> c_dist(0, columns_ - 1);
        int r = r_dist(rng);
        int c = c_dist(rng);

        if (bits_.at(r).at(c))
            return {r, c};
    }
}
