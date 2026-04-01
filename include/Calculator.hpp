#pragma once
#include "Grid.hpp"
#include "Material.hpp"
#include <vector>


class Calculator {
    private :
        float _dt;

    public :
        explicit Calculator(float dt) : _dt(dt) {};

        // GETTER
        float getDt() const { return _dt; }

        // Calculate the temperature at the next time step
        void calcul_temperature(const Grid& grid, const Material& mat, std::vector<float>& T);

        // Necessary condition to avoid divergence (CFL)
        bool isStable(const Grid& grid, const Material& mat) const;

        float maxStableTimeStep(const Grid& grid, const Material& mat) const;
};
