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

using namespace std;

class sphere: public object
{
    public:
        sphere();
        sphere(point loc, double radius, material mtl);
        bool intersect(ray rr, double &t);
        rgb shadeRay(ray rr, double t, vector<light> lights, vector<object*> objects);
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
