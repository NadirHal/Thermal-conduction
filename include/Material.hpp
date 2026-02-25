#pragma once
#include <iostream>

class Material {
    private :
        float k, kho, c;

    public :
        Material(float k, float kho, float c) : k(k), kho(kho), c(c) {}
        float conductivity() const { return k; }
        float density() const { return kho; }
        float heatCapacity() const { return c; }
        float ThermalDiffusity() const { return k / (kho*c); }
}; 