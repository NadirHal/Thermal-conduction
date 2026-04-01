#pragma once
#include <string>
#include <vector>

class Material {
    private :
        std::string _name;
        float _k;    // Thermal conductivity [W/(m·K)]
        float _rho;  // Density [kg/m³]
        float _c;    // Specific heat capacity [J/(kg·K)]

    public :
        Material(const std::string& name, float k, float rho, float c)
            : _name(name), _k(k), _rho(rho), _c(c) {}

        //GETTERS
        std::string name()             const { return _name; }
        float conductivity()           const { return _k; }
        float density()                const { return _rho; }
        float heatCapacity()           const { return _c; }
        float thermalDiffusivity()     const { return _k / (_rho * _c); }  // Alias

        // Preset metals with real physical constants
        static Material Copper()   { return {"Copper",    401.0f,  8960.0f, 385.0f}; }
        static Material Aluminum() { return {"Aluminum",  237.0f,  2700.0f, 900.0f}; }
        static Material Iron()     { return {"Iron",       79.0f,  7874.0f, 449.0f}; }
        static Material Steel()    { return {"Steel",      50.0f,  7850.0f, 502.0f}; }
        static Material Titanium() { return {"Titanium",   22.0f,  4507.0f, 520.0f}; }
        static Material Gold()     { return {"Gold",      317.0f, 19300.0f, 129.0f}; }
        static Material Silver()   { return {"Silver",    429.0f, 10500.0f, 235.0f}; }

        static std::vector<Material> allPresets() {
            return { Copper(), Aluminum(), Iron(), Steel(), Titanium(), Gold(), Silver() };
        }
};
