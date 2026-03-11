#include "../include/NeumannBC.hpp"

void NeumannBC::apply(std::vector<float> &T, int nx, int ny) {
    //Apply Neumann Boundary Condition
    switch(getSide()){
        case Side::South:
            for (int i=0; i<nx; i++) {
                T[i] = T[i + nx];  //Every point on the South boundary is set to the Neumann value
            }
            break;
        case Side::North:
            for (int i=0; i<nx; i++) {
                T[(ny-1)*nx + i] = T[(ny-2)*nx + i];  //Every point on the North boundary is set to the Neumann value
            }
            break;
        case Side::West:
            for (int j=0; j<ny; j++) {
                T[j*nx] = T[j*nx + 1];  //Every point on the West boundary is set to the Neumann value
            }
            break;
        case Side::East:
            for (int j=0; j<ny; j++) {
                T[j*nx + (nx-1)] = T[j*nx + (nx-2)];  //Every point on the East boundary is set to the Neumann value
            }
            break;
    }
};
