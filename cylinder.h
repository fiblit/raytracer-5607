#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
#include "ray.h"
#include "light.h"
#include "material.h"
#include "object.h"
#include "IO.h"
#include "constants.h"
#include "traceRay.h"

class cylinder: public object
{
    public:
        enum class cylTypes : char {X = 'x', Y, Z};
        cylinder();
        cylinder(double u, double v, double radius, double minw, double maxw, cylTypes type, material mtl);
        ~cylinder();
        bool intersect(ray rr, double &t, fileData *fd);
        rgb shadeRay(ray rr, double t, fileData *fd, int depth);//fd for lights, objects(,textures)
        double getRadius();
        double getU();
        double getV();
        double getMaxw();
        double getMinw();
        cylTypes getType();
        material getMtl();
        void setRadius(double r);
        void setU(double u);
        void setV(double v);
        void setMaxw(double maxw);
        void setMinw(double minw);
        void setType(cylTypes type);
        void setMtl(material mtl);
    private:
        double radius;
        double u;
        double v;
        double minw;
        double maxw;
        cylTypes type;
        material mtl;

};

#endif // CYLINDER_H
