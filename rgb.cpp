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

rgb rgb::clamp()
{
    rgb color = *this;
    if (color.getR() > 1.0)
        color.setR(1.0);
    if (color.getG() > 1.0)
        color.setG(1.0);
    if (color.getB() > 1.0)
        color.setB(1.0);
    if (color.getR() < 0.0)
        color.setR(0.0);
    if (color.getG() < 0.0)
        color.setG(0.0);
    if (color.getB() < 0.0)
        color.setB(0.0);
    return color;
}
