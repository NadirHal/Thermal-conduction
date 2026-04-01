#include "../include/DirichletBC.hpp"

// Convention: Grid::index(i,j) = i*ny + j
//   i in [0, nx-1] : column index (X axis)
//   j in [0, ny-1] : row index    (Y axis)
// Fixed indexing: old version used j*nx+i which was inconsistent with Grid::index()

void DirichletBC::apply(std::vector<float>& T, int nx, int ny) {
    //Apply Dirichlet Boundary Condition
    switch (getSide()) {

        case Side::South:   // j = 0 , bottom boundary
            for (int i = 0; i < nx; ++i)
                T[i * ny + 0] = value; //Every point on the South boundary is set to the Dirichlet value
            break;

        case Side::North:   // j = ny-1, top boundary
            for (int i = 0; i < nx; ++i)
                T[i * ny + (ny - 1)] = value;  //Every point on the North boundary is set to the Dirichlet value
            break;

        case Side::West:    // i = 0, left boundary
            for (int j = 0; j < ny; ++j)
                T[0 * ny + j] = value; //Every point on the West boundary is set to the Dirichlet value
            break;

        case Side::East:    // i = nx-1 right boundary
            for (int j = 0; j < ny; ++j)
                T[(nx - 1) * ny + j] = value;  //Every point on the East boundary is set to the Dirichlet value
            break;
    }
}
