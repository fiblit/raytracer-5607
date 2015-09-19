#include "vector3.h"
#include "point.h"//This isn't in vector3.h for compiler reasons.

using namespace std;

vector3::vector3() {}

vector3::vector3(int x, int y, int z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

double vector3::dotProduct(vector3 v)
{
    return this->getX()*v.getX() + this->getY()*v.getY() + this->getZ()*v.getZ();
}

vector3 vector3::crossProduct(vector3 v)
{
    return vector3(this->getY()*v.getZ() - this->getZ()*v.getY(),
        this->getZ()*v.getX() - this->getX()*v.getZ(),
        this->getX()*v.getY() - this->getY()*v.getX());
}

double vector3::length()
{
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

vector3 vector3::scale(double s)
{
    return vector3(s*this->getX(), s*this->getY(), s*this->getZ());
}

vector3 vector3::add(vector3 v)
{
    return vector3(this->getX()+v.getX(), this->getY()+v.getY(), this->getZ()+v.getZ());
}

vector3 vector3::operator+(const vector3 &u)
{
    return this->add(u);
}

vector3 vector3::operator-(const vector3 &u)
{
    vector3 ut = u;//since u is const, and add takes non-const vector3
    return this->add(ut.scale(-1));
}

vector3 vector3::unit()
{
    return this->scale(1/this->length());
}

point vector3::toPoint()
{
    return point(this->getX(), this->getY(), this->getZ());
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
