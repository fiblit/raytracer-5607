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

double rgb::getR()
{
    return r;
}

double rgb::getG()
{
    return g;
}

double rgb::getB()
{
    return b;
}
