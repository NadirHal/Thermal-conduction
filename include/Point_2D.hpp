#pragma once
#include <iostream>
#include <cmath> // sqrt() & pow()

class Point_2D {
    private :
        float x, y;

    public :
        Point_2D(int val_x, int val_y) : x(val_x), y(val_y) {}

        // GETTERS
        float getX() const { return x; }
        float getY() const { return y; }

        // SETTERS
        void setX(float val) { x = val; }
        void setY(float val) { y = val; }

        // overload + and -
        Point_2D operator+(const Point_2D &p) const {
            return Point_2D(x + p.x, y + p.y);
        }

        Point_2D operator-(const Point_2D &p) const {
            return Point_2D(x - p.x, y - p.y);
        }

        // from (0,0)
        float distance() const {
            return std::sqrt(pow(x, 2) + pow(y, 2));
        }

        //from p2
        float distance(const Point_2D &p) const {
            return std::sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
        }

};