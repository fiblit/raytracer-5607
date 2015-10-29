#include "traceRay.h"

void traceRay(ray rr, fileData_t *fd, rgb &color)
{
    vector<object *> objects = *(fd->objects);
    int closest = -1;
    double closestInter = numeric_limits<double>::infinity();
    for(int i = 0; i < (int)objects.size(); i++)//for each object in scene
    {
        double t;
        if(objects[i]->intersect(rr, t, fd) && (closestInter > t))//returns true if intersected, assigns closer (non-neg) intersection to t
        {
            closestInter = t;
            closest = i;
        }
    }
    if (closest!=-1)
        color = (objects[closest]->shadeRay(rr, closestInter, fd));//I'm pretty sure this doesn't work, if not, I can malloc here
}
