#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

// Real-time heatmap renderer using SFML
// Color represents temperature: blue (cold) -> cyan -> green -> yellow -> red (hot)
class Visualizer {
private:
    sf::RenderWindow          window;
    sf::Image                 image;
    sf::Texture               texture;
    std::optional<sf::Sprite> sprite;   // Optional: sf::Sprite has no default constructor in SFML 3
    sf::Font                  font;
    bool                      fontLoaded;

    int   nx, ny;
    int   cellSize; // Pixels per cell
    float T_min, T_max;

    // Converts a normalised temperature [0,1] to a heatmap colour
    sf::Color heatmapColor(float t) const;

public:
    Visualizer(int nx, int ny, int cellSize, float T_min, float T_max);

    bool isOpen() const;
    void processEvents();

    // Update the image and display one frame
    void render(const std::vector<float>& temperature,
                float current_time,
                const std::string& material_name,
                const std::string& unit = "C");
};
