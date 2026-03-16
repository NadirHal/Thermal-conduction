#include "RobinBC.hpp"

RobinBC::RobinBC(Side side, float h_conv, float T_amb)
    : BoundaryCondition(side), h_conv(h_conv), T_amb(T_amb) {}

void RobinBC::apply(std::vector<float>& T, int nx, int ny, float lambda, float dx) {
    float denom = lambda + h_conv * dx;
    
    switch (side) {
        case Side::East:
            for (int j = 0; j < ny; ++j) {
                // On récupère le nœud juste avant la bordure
                float T_inner = T[(nx - 2) * ny + j];
                // Application de la condition discrétisée
                T[(nx - 1) * ny + j] = (lambda * T_inner + h_conv * dx * T_amb) / denom;
            }
            break;

        // Les autres cas (West, North, South) seront à implémenter ici
        default: 
            break;
    }
}

void RobinBC::apply(std::vector<float>& T, int nx, int ny) {
    // Cette méthode est problématique car Robin nécessite lambda et dx.
    // On peut soit lever une exception, soit ne rien faire, 
    // soit logger un avertissement.
}