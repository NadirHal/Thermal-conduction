    #pragma once

class Grid {
    private :
        int _nx, _ny;    // number of cells 
        float _dx, _dy;   // dimensions of a cell

    public :
        
        Grid(int nx, int ny, float dx, float dy) 
            : _nx(nx), _ny(ny), _dx(dx), _dy(dy) {}

        
        int nx() const { return _nx; }
        int ny() const { return _ny; }
        float dx() const { return _dx; }
        float dy() const { return _dy; }

        // FUNCTIONS
        int totalCells() const {
            return _nx * _ny;
        }

        int index(int i, int j) const {
            
            return i * _ny + j;
        }

        bool isValid(int i, int j) const {
            return i >= 0 && i < _nx && j >= 0 && j < _ny;
        }
};
