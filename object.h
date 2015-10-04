#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "rgb.h"
#include "ray.h"
#include "light.h"

using namespace std;

class object
{
    public:
        //object();
        virtual bool intersect(ray rr, double &t)=0;
        //{
        //    return false;
        //}
        virtual rgb shadeRay(ray rr, double t, vector<light> lights, vector<object*> objects)=0;
        //{
        //   return rgb();
        //}
};

#endif // OBJECT_H
