#include "../include/Visualizer.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

// Heatmap palette: blue -> cyan -> green -> yellow -> red
sf::Color Visualizer::heatmapColor(float t) const {
    t = std::clamp(t, 0.0f, 1.0f);
    float r, g, b;
    if      (t < 0.25f) { float s=t/0.25f;          r=0; g=s;   b=1;   }
    else if (t < 0.50f) { float s=(t-0.25f)/0.25f;  r=0; g=1;   b=1-s; }
    else if (t < 0.75f) { float s=(t-0.50f)/0.25f;  r=s; g=1;   b=0;   }
    else                { float s=(t-0.75f)/0.25f;  r=1; g=1-s; b=0;   }
    return sf::Color(uint8_t(r*255), uint8_t(g*255), uint8_t(b*255));
}

Visualizer::Visualizer(int nx, int ny, int cellSize, float T_min, float T_max)
    : nx(nx), ny(ny), cellSize(cellSize), T_min(T_min), T_max(T_max),
      fontLoaded(false)
{
    const unsigned winW = static_cast<unsigned>(nx * cellSize);
    const unsigned winH = static_cast<unsigned>(ny * cellSize + 110);

    window.create(sf::VideoMode({winW, winH}),
                  "Simulation de conduction thermique 2D");
    window.setFramerateLimit(60);

    const unsigned imgW = static_cast<unsigned>(nx * cellSize);
    const unsigned imgH = static_cast<unsigned>(ny * cellSize);

    image.resize({imgW, imgH}, sf::Color::Black);

    if (!texture.resize({imgW, imgH}))
        throw std::runtime_error("Impossible de creer la texture SFML");

    // sf::Sprite requires a texture at construction in SFML 3 (no default constructor)
    sprite = sf::Sprite(texture);

    const std::vector<std::string> fontPaths = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "/Library/Fonts/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "C:/Windows/Fonts/arial.ttf"
    };
    for (auto& path : fontPaths)
        if (font.openFromFile(path)) { fontLoaded = true; break; }
}

bool Visualizer::isOpen() const { return window.isOpen(); }

// Event handling using SFML 3 variant-based API
void Visualizer::processEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
        if (auto* kp = event->getIf<sf::Event::KeyPressed>())
            if (kp->code == sf::Keyboard::Key::Escape)
                window.close();
    }
}

void Visualizer::render(const std::vector<float>& temperature,
                        float current_time,
                        const std::string& material_name,
                        const std::string& unit)
{
    float range = (T_max > T_min) ? (T_max - T_min) : 1.0f;

    // Update pixel colours from temperature values
    // Y axis is flipped so that South (j=0) appears at the bottom of the window
    for (int i = 0; i < nx; ++i)
        for (int j = 0; j < ny; ++j) {
            float t = (temperature[i * ny + j] - T_min) / range;
            sf::Color col = heatmapColor(t);
            int px = i * cellSize;
            int py = (ny - 1 - j) * cellSize;
            for (int di = 0; di < cellSize; ++di)
                for (int dj = 0; dj < cellSize; ++dj)
                    image.setPixel({unsigned(px+di), unsigned(py+dj)}, col);
        }

    texture.update(image);

    const int PANEL_H = 110;
    const int winW    = nx * cellSize;
    const int barW    = winW - 40;
    const int barH    = 18;
    const int barX    = 20;
    const int barY    = ny * cellSize + 10;

    window.clear(sf::Color(10, 10, 20));
    window.draw(*sprite);

    // Info panel background
    sf::RectangleShape panel(sf::Vector2f(winW, PANEL_H));
    panel.setPosition(sf::Vector2f(0.f, float(ny * cellSize)));
    panel.setFillColor(sf::Color(20, 20, 30));
    window.draw(panel);

    // Colour scale bar
    for (int x = 0; x < barW; ++x) {
        sf::RectangleShape seg(sf::Vector2f(1.f, float(barH)));
        seg.setPosition(sf::Vector2f(float(barX + x), float(barY)));
        seg.setFillColor(heatmapColor(float(x) / barW));
        window.draw(seg);
    }
    // Colour bar outline (braces used to avoid most-vexing-parse)
    sf::RectangleShape frame{sf::Vector2f{float(barW), float(barH)}};
    frame.setPosition(sf::Vector2f{float(barX), float(barY)});
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(1.f);
    window.draw(frame);

    // Text labels (only if a font was successfully loaded)
    if (fontLoaded) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << T_min << " deg" << unit;
        sf::Text tMin(font, ss.str(), 12);
        tMin.setFillColor(sf::Color::White);
        tMin.setPosition(sf::Vector2f(float(barX), float(barY + barH + 3)));
        window.draw(tMin);

        ss.str(""); ss << T_max << " deg" << unit;
        sf::Text tMax(font, ss.str(), 12);
        tMax.setFillColor(sf::Color::White);
        float tw = tMax.getLocalBounds().size.x;
        tMax.setPosition(sf::Vector2f(float(barX + barW) - tw, float(barY + barH + 3)));
        window.draw(tMax);

        ss.str("");
        ss << "Materiau : " << material_name
           << "   |   Temps : " << std::setprecision(4) << current_time << " s"
           << "   |   [Echap] pour quitter";
        sf::Text tInfo(font, ss.str(), 13);
        tInfo.setFillColor(sf::Color(200, 200, 200));
        tInfo.setPosition(sf::Vector2f(float(barX), float(barY + barH + 22)));
        window.draw(tInfo);
    }

    window.display();
}
