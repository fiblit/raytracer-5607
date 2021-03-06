#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class point;

class vector3
{
    public:
        vector3();
        vector3(double x, double y, double z);
        double dotProduct(vector3 v);
        vector3 crossProduct(vector3 v);
        double length();
        vector3 unit();
		point toPoint();
        vector3 scale(double s);
        vector3 fscale(double s);
        vector3 add(vector3 v);
        vector3 operator* (const double &s);
        vector3 operator+ (const vector3 &u);
        vector3 operator- (const vector3 &u);
        void setVec(double x, double y, double z);
        double getX() const;
        double getY() const;
        double getZ() const;
    private:
        double x;
        double y;
        double z;
};

#endif
