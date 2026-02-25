#pragma once

class Material {
    private :
        float k, kho, c;

    public :
        Material(float k, float kho, float c);
        float conductivity() const;
        float density() const;
        float heatCapacity() const;
        float ThermalDiffusity() const;
};