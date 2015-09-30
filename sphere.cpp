#include "sphere.h"

sphere::sphere() {}

sphere::sphere(point loc, double radius, material mtl)
{
    this->loc = loc;
    this->radius = radius;
    this->mtl = mtl;
}

//Determines if the ray rr intersects this sphere.
//If so, it will put the closest parametric value, for the equation of rr, into t.
bool sphere::intersect(ray rr, double &t)
{
    point rloc = rr.getLoc();
    vector3 rdir = rr.getDir();
    //Assume a = 1 because rdir is normalized.
    double b = 2 * (rdir.getX() * (rloc.getX() - loc.getX()) + rdir.getY() * (rloc.getY() - loc.getY()) + rdir.getZ() * (rloc.getZ() - loc.getZ()));
    double c = pow(rloc.getX()-loc.getX(),2) + pow(rloc.getY()-loc.getY(),2) + pow(rloc.getZ()-loc.getZ(),2) - pow(radius, 2);

    double discrim = (b * b) - (4 * c);//again assuming a=1

    if (discrim < 0.0)
    {
        return false; //No intersection, ray missed
    }
    double sol1 = ((-b) + sqrt(discrim))/2;
    double sol2 = ((-b) - sqrt(discrim))/2;
    if (sol1 < 0)
    {
        if (sol2 < 0)
            return false; //No intersection, behind us
        t = sol2;
    }
    else if (sol2 < 0)
        t = sol1;
    else if (sol1 <= sol2)
        t = sol1;
    else
        t = sol2;

    return true; //intersection at t
}

rgb sphere::shadeRay(ray rr, double t)
{
    return this->getMaterial().getOd();
}

material sphere::getMaterial()
{
    return mtl;
}

void sphere::setMaterial(material mtl)
{
    this->mtl = mtl;
}

point sphere::getLoc()
{
    return loc;
}

void sphere::setLoc(point p)
{
    loc = p;
}

double sphere::getRadius()
{
    return radius;
}

void sphere::setRadius(double r)
{
    radius = r;
}
