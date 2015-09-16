#include "vector3.h"

using namespace std;

vector3::vector3() { }

vector3::vector3(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}


double vector3::dotProduct(vector3 u, vector3 v)
{
    return u.getX()*v.getX() + u.getY()*v.getY() + u.getZ()*v.getZ();
}

vector3 vector3::crossProduct(vector3 u, vector3 v)
{
    return vector3(u.getY()*v.getZ() - u.getZ()*v.getY(),
        u.getZ()*v.getX() - u.getX()*v.getZ(),
        u.getX()*v.getY() - u.getY()*v.getX());
}

double vector3::length()
{
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

vector3 vector3::scale(double s, vector3 u)
{
    return vector3(s*u.getX(), s*u.getY(), s*u.getZ());
}

vector3 vector3::unit(vector3 u)
{
    return scale(1/u.length(), u);
}

void vector3::setVec(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

double vector3::getX()
{
    return x;
}

double vector3::getY()
{
    return y;
}

double vector3::getZ()
{
    return z;
}
