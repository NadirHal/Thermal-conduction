#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include "Grid.hpp"
#include "Material.hpp"
#include "Calculator.hpp"
#include "BoundaryCondition.hpp"

class Simulation {
private:
    Grid grid;
    Material material;
    Calculator calculator;
    std::vector<float> temperature;
    std::vector<std::unique_ptr<BoundaryCondition>> boundaryConditions;
    float current_time;
    float run_time;
    

public:
    Simulation(int nx, int ny, float cellSize, Material material, float dt)
        : grid(nx, ny, cellSize, cellSize),
          material(material),
          calculator(dt),
          temperature(nx * ny, 0.0f),
          current_time(0.0f), run_time(0.0f)
          
    {
        if (!calculator.isStable(grid, material))
            throw std::runtime_error("dt viole la condition CFL !");
    }

    void addBoundaryCondition(std::unique_ptr<BoundaryCondition> bc) {
        boundaryConditions.push_back(std::move(bc));
    }

    void setInitialTemperature(float T) {
        std::fill(temperature.begin(), temperature.end(), T);
    }

    void setRegionTemperature(int i0, int i1, int j0, int j1, float T) {
        for (int i = i0; i <= i1; ++i)
            for (int j = j0; j <= j1; ++j)
                temperature[grid.index(i,j)] = T;
    }

    void start(int numSteps) {
        for (int n = 0; n < numSteps; ++n) {
            // 1. Calcul du stencil sur l'intérieur
            calculator.calcul_temperature(grid, material, temperature);
            // 2. Application des conditions aux limites
            for (auto& bc : boundaryConditions)
                bc->apply(temperature, grid.nx(), grid.ny());
            current_time += /* dt */ 0.0f; // récupérer dt via getter si besoin
        }
        run_time = current_time;
    }

    const Grid&          getGrid()        const { return grid; }
    const std::vector<float>& getTemperature() const { return temperature; }
    float                getCurrentTime() const { return current_time; }
};