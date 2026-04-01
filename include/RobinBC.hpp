#pragma once
#include "BoundaryCondition.hpp"

// Robin (convection) boundary condition
// Stores lambda and dx internally so it matches the base class interface
class RobinBC : public BoundaryCondition {
    private:
        float h_conv;   // Convection coefficient [W/(m²·K)]
        float T_amb;    // Ambient temperature [°C or K]
        float lambda;   // Material conductivity [W/(m·K)]
        float dx;       // Cell size [m]

    public:
        RobinBC(Side side, float h_conv, float T_amb, float lambda, float dx);
        void apply(std::vector<float>& T, int nx, int ny) override;
};
