#pragma once
#include <vector>
#include <memory>
#include "Grid.hpp"
#include "Material.hpp"
#include "Calculator.hpp"
#include "BoundaryCondition.hpp"

class Simulation {
    private:
        Grid     grid;
        Material material;
        Calculator calculator;
        std::vector<float> temperature;
        std::vector<std::unique_ptr<BoundaryCondition>> boundaryConditions;
        float current_time;
        float run_time;

    public:
        Simulation(int nx, int ny, float cellSize, Material material, float dt);

        void addBoundaryCondition(std::unique_ptr<BoundaryCondition> bc);
        void setInitialTemperature(float T);
        void setRegionTemperature(int i0, int i1, int j0, int j1, float T);

        // Run numSteps iterations and return true if successful
        bool step(int numSteps = 1);

        // GETTERS
        const Grid&               getGrid()        const { return grid; }
        const Material&           getMaterial()    const { return material; }
        const std::vector<float>& getTemperature() const { return temperature; }
        float                     getCurrentTime() const { return current_time; }
        float                     getDt()          const { return calculator.getDt(); }
};
