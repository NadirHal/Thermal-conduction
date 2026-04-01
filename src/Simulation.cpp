#include "../include/Simulation.hpp"
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
    // Check stability (CFL) at construction time
    if (!calculator.isStable(grid, this->material)) {
        float dt_max = calculator.maxStableTimeStep(grid, this->material);
        throw std::runtime_error(
            "Time step dt=" + std::to_string(dt) +
            " violates CFL condition! dt_max=" + std::to_string(dt_max));
    }
}

void Simulation::addBoundaryCondition(std::unique_ptr<BoundaryCondition> bc) {
    boundaryConditions.push_back(std::move(bc));
}

void Simulation::setInitialTemperature(float T) {
    std::fill(temperature.begin(), temperature.end(), T);
}

void Simulation::setRegionTemperature(int i0, int i1, int j0, int j1, float T) {
    for (int i = i0; i <= i1; ++i)
        for (int j = j0; j <= j1; ++j)
            temperature[grid.index(i, j)] = T;
}

bool Simulation::step(int numSteps) {
    const float dt = calculator.getDt();

    for (int n = 0; n < numSteps; ++n) {
        // 1.Update interior points (explicit Euler)
        calculator.calcul_temperature(grid, material, temperature);

        // 2. Apply boundary conditions
        for (auto& bc : boundaryConditions)
            bc->apply(temperature, grid.nx(), grid.ny());

        // 3. Time advancement (fixed : was += 0.0f)
        current_time += dt;
    }

    run_time = current_time;
    return true;
}
