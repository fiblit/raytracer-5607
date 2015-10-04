#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
#include "ray.h"
#include "light.h"
#include "material.h"
#include "object.h"

class cylinder: public object
{
    public:
        enum class cylTypes : char {X = 'x', Y, Z};
        cylinder();
        cylinder(double radius, double u, double v, cylTypes type, material mtl);
        bool intersect(ray rr, double &t);
        rgb shadeRay(ray rr, double t, vector<light> lights, vector<object*> objects);
        double getRadius();
        double getU();
        double getV();
        cylTypes getType();
        material getMtl();
        void setRadius(double r);
        void setU(double u);
        void setV(double v);
        void setType(cylTypes type);
        void setMtl(material mtl);
    private:
        double radius;
        double u;
        double v;
        cylTypes type;
        material mtl;

};

#endif // CYLINDER_H
