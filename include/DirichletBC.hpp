#pragma once
#include "BoundaryCondition.hpp"

// Condition de Dirichlet : température imposée sur un bord
class DirichletBC : public BoundaryCondition {
private:
    float value;   // Température imposée [°C ou K]

public:
    DirichletBC(Side s, float v) : BoundaryCondition(s), value(v) {}
    float getValue() const { return value; }
    void apply(std::vector<float>& T, int nx, int ny) override;
};
