#ifndef SPHERE_H
#define SPHERE_H

#include "material.h"
#include "rgb.h"
#include "point.h"
#include "ray.h"
#include <cmath>

using namespace std;

class sphere
{
    public:
        sphere();
        sphere(point loc, double radius, material mtl);
        bool intersect(ray rr, double &t);
        rgb shadeRay(ray rr, double t);
        material getMaterial();
        void setMaterial(material mtl);
        point getLoc();
        void setLoc(point p);
        double getRadius();
        void setRadius(double r);
    private:
        point loc;
        double radius;
        //rgb color;
        material mtl;
};

#endif // SPHERE_H
