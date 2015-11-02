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

point point::operator+(const vector3 &u)
{
    return point(x + u.getX(), y + u.getY(), z + u.getZ());
}

point point::jitter(double jit)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);
    std::uniform_real_distribution<double> distribution(-jit, jit);
    auto delta = std::bind (distribution, generator);
    double dx = delta();
    double dy = delta();
    double dz = delta();
    return point(this->getX() + dx, this->getY() + dy, this->getZ() + dz);
}
