#pragma once
#include "BoundaryCondition.hpp"

class RobinBC : public BoundaryCondition {
private:
    float h_conv, T_amb;

public:
    RobinBC(Side side, float h_conv, float T_amb)
        : BoundaryCondition(side), h_conv(h_conv), T_amb(T_amb) {}

    // Formule discrétisée : T_bord = (λ*T_voisin + h*dx*T_amb) / (λ + h*dx)
    void apply(std::vector<float>& T, int nx, int ny,
               float lambda, float dx) {
        float denom = lambda + h_conv * dx;
        switch (side) {
            case Side::East:
                for (int j = 0; j < ny; ++j) {
                    float T_inner = T[(nx-2)*ny+j];
                    T[(nx-1)*ny+j] = (lambda*T_inner + h_conv*dx*T_amb) / denom;
                } break;
            // Adapter symétriquement pour les autres faces...
            default: break;
        }
    }

    // Surcharge de l'interface de base (flux nul par défaut si λ non fourni)
    void apply(std::vector<float>& T, int nx, int ny) override {
        // Nécessite lambda et dx — appeler la surcharge ci-dessus
    }
};