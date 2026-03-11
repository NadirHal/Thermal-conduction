#pragma once
#include "Grid.hpp"
#include "Material.hpp"
#include <vector>


class Calculator {
    private :
        float _dt;

    public :
        Calculator(float _dt) : _dt(_dt) {};

        // GETTER
        float dt() const {return _dt; }

        // calculate the temperature at the next time step
        void calcul_temperature(const Grid &grid, const Material &mat, std::vector<float>& T);


        // stability conditions
        bool is_stable(const Grid& grid, const Material& mat) const;

        float maxStableTimeStep(const Grid& grid, const Material& mat) const;
        

};