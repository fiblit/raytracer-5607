#ifndef POINT_H
#define POINT_H

#include "vector3.h"

class point
{
    public:
        point();
		point(double x, double y, double z);
		vector3 vect();
		vector3 subtract(point p);
		void setX(double x);
		void setY(double y);
		void setZ(double z);
		double getX();
		double getY();
		double getZ();
    protected:
    private:
		double x;
		double y;
		double z;
};

#endif // POINT_H
