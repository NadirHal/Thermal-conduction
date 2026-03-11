#pragma once // To avoid having multiples inclusions
#include <iostream>
#include "BoundaryCondition.hpp"

class DirichletBC: public BoundaryCondition {
    private:
    float value; 

    public:
    DirichletBC(Side s,float v): BoundaryCondition(s), value(v){}; // Constructor + inline definition 

    void apply(std::vector<float> &T, int nx, int ny) override {} // Redefining the function that's in the mother class. Override bc we've already defined it 

};