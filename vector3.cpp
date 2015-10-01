#include "vector3.h"
#include "point.h"//This isn't in vector3.h for compiler reasons.

using namespace std;

vector3::vector3() {}

vector3::vector3(double x, double y, double z)
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
    return sqrt(pow(this->getX(),2) + pow(this->getY(),2) + pow(this->getZ(),2));
}

vector3 vector3::scale(double s)
{
    return vector3(s*(this->getX()), s*(this->getY()), s*(this->getZ()));
}

//This function may seem redundant, but I have it for two (three) reasons.
//One it was a fix to a symptom of a bug, (I fixed the root of the bug).
//Two it *may* help with numerical error. ((1/big#2)*big#1 might be 0, but big#1/big#2 might be some#, instead of 0)
//Three it is due for deprecation, I just will be doing that in homework 1b XD
vector3 vector3::fscale(double s)
{
    return vector3((this->getX())/s, (this->getY())/s, (this->getZ())/s);
}

vector3 vector3::add(vector3 v)
{
    return vector3(this->getX()+v.getX(), this->getY()+v.getY(), this->getZ()+v.getZ());
}

vector3 vector3::operator*(const double &s)
{
    double t = s;//scale takes non-const. (it should be const...)
    return this->scale(t);
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
    return this->fscale(this->length());
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

double vector3::getX() const
{
    return x;
}

double vector3::getY() const
{
    return y;
}

double vector3::getZ() const
{
    return z;
}
