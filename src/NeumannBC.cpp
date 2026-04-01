#include "../include/NeumannBC.hpp"


void NeumannBC::apply(std::vector<float>& T, int nx, int ny) {
    //Apply Neumann Boundary Condition (zero flux: dT/dn = 0)
    //Each boundary node takes the value of its nearest interior neighbour

    switch (getSide()) {

        case Side::South:   // j = 0, copy from j = 1
            for (int i = 0; i < nx; ++i)
                T[i * ny + 0] = T[i * ny + 1];
            break;

        case Side::North:   // j = ny-1, copy from j = ny-2
            for (int i = 0; i < nx; ++i)
                T[i * ny + (ny - 1)] = T[i * ny + (ny - 2)];
            break;

        case Side::West:    // i = 0, copy from i = 1
            for (int j = 0; j < ny; ++j)
                T[0 * ny + j] = T[1 * ny + j];
            break;

        case Side::East:    // i = nx-1, copy from i = nx-2
            for (int j = 0; j < ny; ++j)
                T[(nx - 1) * ny + j] = T[(nx - 2) * ny + j];
            break;
    }
}
