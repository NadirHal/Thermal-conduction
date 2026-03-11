#pragma once // To avoid having multiples inclusions
#include <vector>
#include "Side.hpp"


class BoundaryCondition{
    protected:
        Side side; // I have to define in another file the enum Side.
    

    public:
<<<<<<< Updated upstream
        BoundaryCondition(Side side); // Constructor
=======
        BoundaryCondition(Side s):side(s){}; // Constructor + inline definition
>>>>>>> Stashed changes
        virtual ~BoundaryCondition()= default; // Virtual Destructor
        virtual void apply(std::vector<float> &T, int nx, int ny)=0; // pure virtual function
        Side getSide() const { return side; } 

};