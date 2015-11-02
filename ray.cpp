#include "ray.h"

ray::ray() {}

ray::ray(point loc, vector3 dir)
{
    this->loc = loc;
    this->dir = dir;
}

point ray::getLoc()
{
    return loc;
}

void ray::setLoc(point p)
{
    loc = p;
}

vector3 ray::getDir()
{
    return dir;
}

void ray::setDir(vector3 dir)
{
    this->dir = dir;
}

ray ray::jitter(double jit)
{
    return ray(this->getLoc().jitter(jit), this->getDir());
}
