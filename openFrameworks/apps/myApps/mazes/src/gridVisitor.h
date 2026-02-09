#pragma once

#include <random>

struct DistanceGrid;
struct ColorGrid;
struct MaskedGrid;

struct GridVisitor {
    virtual ~GridVisitor() = default;
    virtual void visit(DistanceGrid&, std::mt19937& rng) = 0;
    virtual void visit(ColorGrid&, std::mt19937& rng) = 0;
    virtual void visit(MaskedGrid&, std::mt19937& rng) = 0;
};
