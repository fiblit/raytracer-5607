#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <cmath>
#include "material.h"
#include "rgb.h"
#include "point.h"
#include "ray.h"
#include "light.h"
#include "object.h"
#include "texture.h"
#include "IO.h"

using namespace std;

double const PI = 3.14159265358979323846264338327950288;

class sphere: public object
{
    public:
        sphere();
        sphere(point loc, double radius, material mtl, int texIndex);
        bool intersect(ray rr, double &t, fileData_t *fd);
        rgb shadeRay(ray rr, double t, fileData_t *fd);//fd for lights, objects, textures
        point getLoc();
        double getRadius();
        material getMtl();
        int getTexIndex();
        void setLoc(point p);
        void setRadius(double r);
        void setMtl(material mtl);
        void setTexIndex(int i);
    private:
        point loc;
        double radius;
        material mtl; // TODO: change to mtlIndex
        int texIndex;
};

#endif // SPHERE_H
