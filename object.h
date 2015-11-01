#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "rgb.h"
#include "ray.h"
#include "light.h"
#include "texture.h"
#include "IO.h"
#include "material.h"

using namespace std;

class object
{
    public:
        virtual ~object();
        virtual bool intersect(ray rr, double &t, fileData_t *fd)=0;
        virtual rgb shadeRay(ray rr, double t, fileData_t *fd, int depth)=0;
        virtual material getMtl()=0;
};

#endif // OBJECT_H
