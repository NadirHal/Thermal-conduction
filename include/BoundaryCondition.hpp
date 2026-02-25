#pragma once // To avoid having multiples inclusions
#include <iostream>
#include <vector>
#include "Side.hpp"


class BoundaryCondition{
    protected:
    Side side; // I have to define in another file the enum Side.
    

    public:
    BoundaryCondition(Side side); // Constructor
    virtual ~BoundaryCondition()= default; // Virtual Destructor
    virtual void apply(std::vector<float> &T, int nx, int ny)=0; // pure virtual function
    Side getSide() const { return side; } 

};