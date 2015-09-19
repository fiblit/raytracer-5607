#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class point;

class vector3
{
    public:
        vector3();
        vector3(int x, int y, int z);
        double dotProduct(vector3 v);
        vector3 crossProduct(vector3 v);
        double length();
        vector3 unit();
		point toPoint();
        vector3 scale(double s);
        vector3 scaler(double s, vector3 u);
        vector3 fscale(double s);
        vector3 add(vector3 v);
        vector3 operator+ (const vector3 &u);
        vector3 operator- (const vector3 &u);
        void setVec(double x, double y, double z);
        double getX();
        double getY();
        double getZ();
    private:
        double x;
        double y;
        double z;
};

#endif
