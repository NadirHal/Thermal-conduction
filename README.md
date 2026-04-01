# Thermal-conduction
A progressive C++ framework for simulating 1D, 2D, and 3D heat transfer using object-oriented numerical solvers.

Version 1.0
To start the simualation you'll need SFML Library :

  cmake -B build -DCMAKE_BUILD_TYPE=Release \
    -DSFML_DIR=$(brew --prefix sfml)/lib/cmake/SFML (for MacOS)
    
  cmake --build build
  
  ./build/thermal_sim
  
