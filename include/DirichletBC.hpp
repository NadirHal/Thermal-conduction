#pragma once // To avoid having multiples inclusions
#include <iostream>
#include "BoundaryCondition.hpp"

class DirichletBC: public BoundaryCondition {
    private:
    float value; 

    public:
    DirichletBC(Side s,float v): BoundaryCondition(s), value(v){}; // Constructor + inline definition 

    void apply(std::vector<float> &T, int nx, int ny) override { // Redefining the function that's in the mother class. Override bc we've already defined it
    //Apply Dirichlet Boundary Condition
    switch(getSide()){
        case Side::South:
            for (int i=0; i<nx; i++) {
                T[i] = value; 
            }
            break;
        case Side::North:
            for (int i=0; i<nx; i++) {
                T[(ny-1)*nx + i] = value; 
            }
            break;
        case Side::West:
            for (int j=0; j<ny; j++) {
                T[j*nx] = value; 
            }
            break;
        case Side::East:
            for (int j=0; j<ny; j++) {
                T[j*nx + (nx-1)] = value; 
            }
            break;
    }

        


    }

};