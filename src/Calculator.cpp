#include "../include/Calculator.hpp"
#include <algorithm>

// Grid temperature at the next time step (explicit Euler & Laplacian)
// Loop starts at i=1, j=1 to avoid out-of-bounds access at index -1
// Loop ends at nx-2, ny-2 because boundaries are handled by BoundaryCondition

void Calculator::calcul_temperature(const Grid& grid, const Material& mat,
                                     std::vector<float>& T)
{
    const int   nx = grid.nx();
    const int   ny = grid.ny();
    const float dx = grid.dx();
    const float dy = grid.dy();
    const float D  = mat.thermalDiffusivity();

    std::vector<float> T_next = T;   // Copy: we write into T_next and read from T

    // Only interior points are updated (5-point stencil)
    for (int i = 1; i < nx - 1; ++i) {
        for (int j = 1; j < ny - 1; ++j) {
            float T_ij  = T[grid.index(i,   j)];
            float T_ip1 = T[grid.index(i+1, j)];
            float T_im1 = T[grid.index(i-1, j)];
            float T_jp1 = T[grid.index(i,   j+1)];
            float T_jm1 = T[grid.index(i,   j-1)];

            float lap_x = (T_ip1 - 2.0f * T_ij + T_im1) / (dx * dx);
            float lap_y = (T_jp1 - 2.0f * T_ij + T_jm1) / (dy * dy);

            T_next[grid.index(i, j)] = T_ij + _dt * D * (lap_x + lap_y);
        }
    }

    T = std::move(T_next);
}

// Necessary condition to avoid divergence (CFL)
// Fixed formula: (1.0f/(dy*dy)) instead of (1.0f/dy*dy)
float Calculator::maxStableTimeStep(const Grid& grid, const Material& mat) const {
    const float D  = mat.thermalDiffusivity();
    const float dx = grid.dx();
    const float dy = grid.dy();

    
    float dt_max = 1.0f / (2.0f * D * (1.0f / (dx * dx) + 1.0f / (dy * dy)));
    return dt_max;
}

bool Calculator::isStable(const Grid& grid, const Material& mat) const {
    return _dt <= maxStableTimeStep(grid, mat);
}
