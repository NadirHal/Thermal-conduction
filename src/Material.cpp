#include <iostream>
#include "Material.hpp"


Material::Material(float k, float kho, float c) : k(k), kho(kho), c(c) {};

float Material::conductivity() const { return k; }
float Material::density() const { return kho; }
float Material::heatCapacity() const { return c; }
float Material::ThermalDiffusity() const { return k / (kho*c); }