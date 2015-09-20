#include "point.h"

point::point() { }

point::point(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

//vect is shorthand for subtracting origin.
vector3 point::vect()
{
    return vector3(this->x, this->y, this->z);
}

vector3 point::subtract(point p)
{
    return vector3(this->x - p.getX(), this->y - p.getY(), this->z - p.getZ());
}

void point::setX(double x)
{
	this->x = x;
}

double point::getX()
{
	return x;
}
void point::setY(double y)
{
	this->y = y;
}

double point::getY()
{
	return y;
}

void point::setZ(double z)
{
	this->z = z;
}

double point::getZ()
{
	return z;
}
