#include "../include/RobinBC.hpp"

// Robin (convection) boundary condition: -lambda * dT/dn = h_conv * (T - T_amb)
// Discretised form: T_boundary = (lambda * T_inner + h_conv * dx * T_amb) / (lambda + h_conv * dx)

RobinBC::RobinBC(Side side, float h_conv, float T_amb, float lambda, float dx)
    : BoundaryCondition(side), h_conv(h_conv), T_amb(T_amb),
      lambda(lambda), dx(dx) {}

void RobinBC::apply(std::vector<float>& T, int nx, int ny) {
    float denom = lambda + h_conv * dx;

    switch (getSide()) {

        case Side::East:    // i = nx-1, inner neighbour at i = nx-2
            for (int j = 0; j < ny; ++j) {
                float T_inner = T[(nx - 2) * ny + j];
                T[(nx - 1) * ny + j] = (lambda * T_inner + h_conv * dx * T_amb) / denom;
            }
            break;

        case Side::West:    // i = 0, inner neighbour at i = 1
            for (int j = 0; j < ny; ++j) {
                float T_inner = T[1 * ny + j];
                T[0 * ny + j] = (lambda * T_inner + h_conv * dx * T_amb) / denom;
            }
            break;

        case Side::North:   // j = ny-1, inner neighbour at j = ny-2
            for (int i = 0; i < nx; ++i) {
                float T_inner = T[i * ny + (ny - 2)];
                T[i * ny + (ny - 1)] = (lambda * T_inner + h_conv * dx * T_amb) / denom;
            }
            break;

        case Side::South:   // j = 0, inner neighbour at j = 1
            for (int i = 0; i < nx; ++i) {
                float T_inner = T[i * ny + 1];
                T[i * ny + 0] = (lambda * T_inner + h_conv * dx * T_amb) / denom;
            }
            break;
    }
}
