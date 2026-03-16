#include "Simulation.hpp"
#include <stdexcept>
#include <algorithm>

Simulation::Simulation(int nx, int ny, float cellSize, Material material, float dt)
    : grid(nx, ny, cellSize, cellSize),
      material(material),
      calculator(dt),
      temperature(nx * ny, 0.0f),
      current_time(0.0f), 
      run_time(0.0f)
{
    // Vérification de la stabilité (CFL) dès la construction
    if (!calculator.isStable(grid, material)) {
        throw std::runtime_error("Erreur : Le pas de temps (dt) viole la condition CFL !");
    }
}

void Simulation::addBoundaryCondition(std::unique_ptr<BoundaryCondition> bc) {
    boundaryConditions.push_back(std::move(bc));
}

void Simulation::setInitialTemperature(float T) {
    std::fill(temperature.begin(), temperature.end(), T);
}

void Simulation::setRegionTemperature(int i0, int i1, int j0, int j1, float T) {
    for (int i = i0; i <= i1; ++i) {
        for (int j = j0; j <= j1; ++j) {
            temperature[grid.index(i, j)] = T;
        }
    }
}

void Simulation::start(int numSteps) {
    float dt = calculator.getDt(); // On récupère le dt via le calculator

    for (int n = 0; n < numSteps; ++n) {
        // 1. Calcul du stencil sur l'intérieur (Physique)
        calculator.calcul_temperature(grid, material, temperature);

        // 2. Application des conditions aux limites (Contraintes)
        for (auto& bc : boundaryConditions) {
            bc->apply(temperature, grid.nx(), grid.ny());
        }

        current_time += dt;
    }
    run_time = current_time;
}