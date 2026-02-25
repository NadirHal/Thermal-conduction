#pragma once

class Grid {
    private :
        int nx, ny;     // number of cells
        float dx, dy;   // dimensions of a cell

    public :
        Grid(int _nx, int _ny, float _dx, float _dy) : nx(_nx), ny(_ny), dx(_dx), dy(_dy) {}

        // GETTERS
        int nx() const { return nx; }
        int ny() const { return ny; }
        float dx() const { return dx; }
        float dy() const { return dy; }

        // FUNCTIONS
        int totalCells() const {
            return nx * ny;
        }

        int index(int i, int j) const {
            return i * ny + j;
        }

        bool isValid(int i, int j) const {
            return i >= 0 && i < nx && j >= 0 && j < ny;
        }
};