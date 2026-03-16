#pragma once
#include "BoundaryCondition.hpp"
#include <vector>

class RobinBC : public BoundaryCondition {
private:
    float h_conv;
    float T_amb;

public:
    // Constructeur
    RobinBC(Side side, float h_conv, float T_amb);

    // Surcharge principale avec paramètres physiques
    void apply(std::vector<float>& T, int nx, int ny, float lambda, float dx);

    // Surcharge de l'interface de base (implémentation obligatoire si virtuelle pure)
    void apply(std::vector<float>& T, int nx, int ny) override;
};
