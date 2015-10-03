#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <cmath>
#include "material.h"
#include "rgb.h"
#include "point.h"
#include "ray.h"
#include "light.h"

using namespace std;

class sphere
{
    public:
        sphere();
        sphere(point loc, double radius, material mtl);
        bool intersect(ray rr, double &t);
        rgb shadeRay(ray rr, double t, vector<light> lights, vector<sphere> spheres);
        material getMtl();
        void setMtl(material mtl);
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
