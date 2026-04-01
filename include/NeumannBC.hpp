#pragma once
#include "BoundaryCondition.hpp"

// Condition de Neumann : flux nul (bord isolé, dT/dn = 0)
// Implémentation : copie du nœud voisin intérieur
class NeumannBC : public BoundaryCondition {
public:
    explicit NeumannBC(Side s) : BoundaryCondition(s) {}
    void apply(std::vector<float>& T, int nx, int ny) override;
};
