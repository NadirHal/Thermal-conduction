#include <vector>
#include "../include/Grid.hpp"
#include "../include/Material.hpp"
#include "../include/Calculator.hpp"

// Grid temperature at the next time step (explicite Euler & laplacien)
void Calculator::calcul_temperature(const Grid& grid, const Material& mat, std::vector<float>& T){
    int dx = grid.dx(), dy = grid.dy(), nx = grid.nx(), ny = grid.ny();
    float D = mat.ThermalDiffusity();
    std::vector<float> T_next = T;


    for(int i = 0; i < nx - 1; i++){
        for(int j = 0; j < ny - 1; j++){
            float laplacien_x = (T[grid.index(i + 1, j)] - 2.0f * T[grid.index(i, j)] + T[grid.index(i - 1, j)]) / (dx*dx);
            float laplacien_y = (T[grid.index(i, j + 1)] - 2.0f * T[grid.index(i, j)] + T[grid.index(i, j - 1)]) / (dy*dy);

            T_next[grid.index(i, j)] = T[grid.index(i, j)] + dt() * D * (laplacien_x + laplacien_y); 
        }
    }
    T = T_next;
}


float Calculator::maxStableTimeStep(const Grid& grid, const Material& mat) const{
    float D = mat.ThermalDiffusity();
    float dx = grid.dx(), dy = grid.dy();

    float dt_max = 1.0f / ((2.0f * D) * ((1.0f/(dx*dx)) + (1.0f/dy*dy)));

    return dt_max;
}


// Necessary condition to avoid divergence (CFL)
bool Calculator::is_stable(const Grid& grid, const Material& mat) const {
    float dt_max = maxStableTimeStep(grid, mat);

    return dt() <= dt_max;
}