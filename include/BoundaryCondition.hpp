#pragma once
#include <vector>
#include "Side.hpp"

class BoundaryCondition {
protected:
    Side side;

public:
    explicit BoundaryCondition(Side s) : side(s) {}
    virtual ~BoundaryCondition() = default;

    virtual void apply(std::vector<float>& T, int nx, int ny) = 0;
    Side getSide() const { return side; }
};
