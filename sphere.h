#ifndef SPHERE_H
#define SPHERE_H

#include "rgb.h"
#include "point.h"
#include "ray.h"
#include <cmath>

using namespace std;

class sphere
{
    public:
        sphere();
        sphere(point loc, double radius, rgb color);
        bool intersect(ray rr, double &t);
        rgb getColor();
        void setColor(rgb color);
        point getLoc();
        void setLoc(point p);
        double getRadius();
        void setRadius(double r);
    private:
        point loc;
        double radius;
        rgb color;
};

#endif // SPHERE_H
