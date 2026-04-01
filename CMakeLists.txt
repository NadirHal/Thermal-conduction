cmake_minimum_required(VERSION 3.16)
project(ThermalSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- SFML ---
# Sur Ubuntu/Debian : sudo apt install libsfml-dev
# Sur macOS         : brew install sfml
# Sur Windows       : télécharger SFML et adapter SFML_DIR
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# --- Sources ---
add_executable(thermal_sim
    src/main.cpp
    src/Calculator.cpp
    src/DirichletBC.cpp
    src/NeumannBC.cpp
    src/RobinBC.cpp
    src/Simulation.cpp
    src/Visualizer.cpp
)

target_include_directories(thermal_sim PRIVATE include)
target_link_libraries(thermal_sim
    SFML::Graphics
    SFML::Window
    SFML::System
)

# Optimisation en Release
target_compile_options(thermal_sim PRIVATE
    $<$<CONFIG:Release>:-O2>
)
