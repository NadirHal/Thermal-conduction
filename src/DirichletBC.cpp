#include "../include/DirichletBC.hpp"

void DirichletBC::apply(std::vector<float> &T, int nx, int ny) {
    //Apply Dirichlet Boundary Condition
    switch(getSide()){
        case Side::South:
            for (int i=0; i<nx; i++) {
                T[i] = value;  //Every point on the South boundary is set to the Dirichlet value
            }
            break;
        case Side::North:
            for (int i=0; i<nx; i++) {
                T[(ny-1)*nx + i] = value;  //Every point on the North boundary is set to the Dirichlet value
            }
            break;
        case Side::West:
            for (int j=0; j<ny; j++) {
                T[j*nx] = value;  //Every point on the West boundary is set to the Dirichlet value
            }
            break;
        case Side::East:
            for (int j=0; j<ny; j++) {
                T[j*nx + (nx-1)] = value;  //Every point on the East boundary is set to the Dirichlet value
            }
            break;
    }
};
        