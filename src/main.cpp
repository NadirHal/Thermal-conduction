#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "../include/Simulation.hpp"
#include "../include/Visualizer.hpp"
#include "../include/DirichletBC.hpp"
#include "../include/NeumannBC.hpp"
#include "../include/RobinBC.hpp"
#include "../include/Material.hpp"

// -------------------------------------------------------
// Helper: print a separator line
// -------------------------------------------------------
static void printSeparator(char c = '-', int w = 55) {
    std::cout << std::string(w, c) << "\n";
}

// Helper: ask the user for a value within [min_val, max_val]
template<typename T>
static T ask(const std::string& prompt, T min_val, T max_val) {
    T val;
    while (true) {
        std::cout << "  " << prompt << " : ";
        if (std::cin >> val && val >= min_val && val <= max_val)
            return val;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "  Valeur invalide. Entrez un nombre entre "
                  << min_val << " et " << max_val << ".\n";
    }
}

// -------------------------------------------------------
// Material selection menu
// -------------------------------------------------------
static Material chooseMaterial() {
    auto presets = Material::allPresets();

    printSeparator('=');
    std::cout << "  CHOIX DU MATÉRIAU\n";
    printSeparator();
    for (int i = 0; i < (int)presets.size(); ++i) {
        auto& m = presets[i];
        std::cout << "  [" << (i+1) << "] " << std::left << std::setw(12) << m.name()
                  << "  k=" << std::setw(5) << m.conductivity()   << " W/(m·K)"
                  << "  ρ=" << std::setw(6) << m.density()        << " kg/m³"
                  << "  c=" << std::setw(5) << m.heatCapacity()   << " J/(kg·K)"
                  << "  α=" << std::scientific << std::setprecision(2)
                             << m.thermalDiffusivity() << " m²/s\n";
    }
    printSeparator();

    int choice = ask<int>("Votre choix", 1, (int)presets.size());
    return presets[choice - 1];
}

// -------------------------------------------------------
// Boundary condition types
// -------------------------------------------------------
enum class BCType { Dirichlet, Neumann, Robin };

struct BCConfig {
    BCType type;
    float  temp;      // Used for Dirichlet
    float  h_conv;    // Used for Robin
    float  T_amb;     // Used for Robin
};

// Ask the user which boundary condition to apply on a given side
static BCConfig askBC(const std::string& sideName) {
    std::cout << "\n  -- Côté " << sideName << " --\n";
    std::cout << "  [1] Dirichlet (température imposée)\n";
    std::cout << "  [2] Neumann   (flux nul / bord isolé)\n";
    std::cout << "  [3] Robin     (convection avec l'air)\n";

    int type = ask<int>("Type de condition", 1, 3);
    BCConfig cfg;
    switch (type) {
        case 1:
            cfg.type = BCType::Dirichlet;
            cfg.temp = ask<float>("Température imposée (°C)", -200.0f, 5000.0f);
            break;
        case 2:
            cfg.type = BCType::Neumann;
            break;
        case 3:
            cfg.type = BCType::Robin;
            cfg.h_conv = ask<float>("Coefficient de convection h [W/(m²·K)]", 1.0f, 1e6f);
            cfg.T_amb  = ask<float>("Température ambiante (°C)", -100.0f, 2000.0f);
            break;
    }
    return cfg;
}

// -------------------------------------------------------
// MAIN
// -------------------------------------------------------
int main() {
    std::cout << "\n";
    printSeparator('=');
    std::cout << "  2D THERMAL CONDUCTION SIMULATION\n";
    printSeparator('=');
    std::cout << "\n";

    // 1. Choose material
    Material mat = chooseMaterial();
    std::cout << "\n  -> Material: " << mat.name()
              << "  (alpha = " << std::scientific << std::setprecision(3)
              << mat.thermalDiffusivity() << " m²/s)\n\n";

    // 2. Grid parameters
    printSeparator('=');
    std::cout << "  GRID PARAMETERS\n";
    printSeparator();
    int   nx       = ask<int>  ("Number of cells in X (nx)", 10, 300);
    int   ny       = ask<int>  ("Number of cells in Y (ny)", 10, 300);
    float cellSize = ask<float>("Cell size (m)",     0.0001f, 10.0f);

    // Compute and display the CFL maximum time step
    {
        Grid   g_tmp(nx, ny, cellSize, cellSize);
        Calculator c_tmp(1.0f);
        float dt_max = c_tmp.maxStableTimeStep(g_tmp, mat);
        std::cout << "\n  dt_max (CFL) = " << std::scientific << std::setprecision(4)
                  << dt_max << " s\n";
    }

    float dt_max;
    {
        Grid g_tmp(nx, ny, cellSize, cellSize);
        Calculator c_tmp(1.0f);
        dt_max = c_tmp.maxStableTimeStep(g_tmp, mat);
    }
    float dt = ask<float>("Time step dt (s, must be < dt_max)", 1e-12f, dt_max);

    // 3. Initial temperature
    printSeparator('=');
    std::cout << "  INITIAL CONDITIONS\n";
    printSeparator();
    float T_init = ask<float>("Uniform initial temperature (°C)", -200.0f, 5000.0f);

    // 4. Boundary conditions
    printSeparator('=');
    std::cout << "  BOUNDARY CONDITIONS\n";
    printSeparator();
    BCConfig bcSouth = askBC("South (bottom)");
    BCConfig bcNorth = askBC("North (top)");
    BCConfig bcWest  = askBC("West (left)");
    BCConfig bcEast  = askBC("East (right)");

    // 5. Simulation duration
    printSeparator('=');
    std::cout << "  SIMULATION DURATION\n";
    printSeparator();
    float T_total   = ask<float>("Total simulation duration (s)", dt, 1e10f);
    int   steps_per_frame = ask<int>("Time steps per displayed frame", 1, 100000);

    // 6. Cell size in pixels
    int cellPx = ask<int>("Cell size in pixels (1-8)", 1, 8);

    // -------------------------------------------------------
    // Build the simulation
    // -------------------------------------------------------
    std::cout << "\n  Initialisation...\n";

    Simulation sim(nx, ny, cellSize, mat, dt);
    sim.setInitialTemperature(T_init);

    // Determine colour scale bounds from boundary temperatures
    float T_vis_min = T_init, T_vis_max = T_init;

    auto addBC = [&](Side side, const BCConfig& cfg) {
        switch (cfg.type) {
            case BCType::Dirichlet:
                sim.addBoundaryCondition(
                    std::make_unique<DirichletBC>(side, cfg.temp));
                T_vis_min = std::min(T_vis_min, cfg.temp);
                T_vis_max = std::max(T_vis_max, cfg.temp);
                break;
            case BCType::Neumann:
                sim.addBoundaryCondition(
                    std::make_unique<NeumannBC>(side));
                break;
            case BCType::Robin:
                sim.addBoundaryCondition(
                    std::make_unique<RobinBC>(side, cfg.h_conv, cfg.T_amb,
                                              mat.conductivity(), cellSize));
                T_vis_min = std::min(T_vis_min, cfg.T_amb);
                T_vis_max = std::max(T_vis_max, cfg.T_amb);
                break;
        }
    };

    addBC(Side::South, bcSouth);
    addBC(Side::North, bcNorth);
    addBC(Side::West,  bcWest);
    addBC(Side::East,  bcEast);

    if (T_vis_min >= T_vis_max) T_vis_max = T_vis_min + 1.0f;

    // -------------------------------------------------------
    // SFML visualiser
    // -------------------------------------------------------
    Visualizer viz(nx, ny, cellPx, T_vis_min, T_vis_max);
    long long total_steps = static_cast<long long>(T_total / dt);

    std::cout << "\n  Simulation started — " << total_steps << " steps in total.\n";
    std::cout << "  Press [Esc] or close the window to stop.\n\n";

    long long step_count = 0;
    while (viz.isOpen() && step_count < total_steps) {
        viz.processEvents();

        int to_run = static_cast<int>(
            std::min((long long)steps_per_frame, total_steps - step_count));
        sim.step(to_run);
        step_count += to_run;

        viz.render(sim.getTemperature(), sim.getCurrentTime(), mat.name());
    }

    std::cout << "  Simulation finished. Simulated time: "
              << sim.getCurrentTime() << " s\n\n";
    return 0;
}
