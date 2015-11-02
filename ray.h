#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector3.h"

class ray
{
    public:
        ray();
        ray(point loc, vector3 dir);
        point getLoc();
        void setLoc(point p);
        vector3 getDir();
        void setDir(vector3 dir);
        ray jitter(double jit);
    private:
        point loc;
        vector3 dir;
};

#endif // RAY_H
