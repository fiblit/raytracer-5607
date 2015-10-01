#include "rgb.h"

rgb::rgb() {}

rgb::rgb(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void rgb::setRGB(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void rgb::setR(double r)
{
    this->r = r;
}

void rgb::setG(double g)
{
    this->g = g;
}
void rgb::setB(double b)
{
    this->b = b;
}

double rgb::getR() const
{
    return r;
}

double rgb::getG() const
{
    return g;
}

double rgb::getB() const
{
    return b;
}

rgb rgb::operator* (const double &s)
{
    return rgb(r * s, g * s, b * s);
}

rgb rgb::operator* (const rgb &c)
{
    return rgb(r * c.getR(), g * c.getG(), b * c.getB());
}

rgb rgb::operator+ (const rgb &c)
{
    return rgb(r + c.getR(), g + c.getG(), b + c.getB());
}
