#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

using namespace std;

class vector3
{
    public:
        vector3();
        vector3(int x, int y, int z);
        double dotProduct(vector3 u, vector3 v);
        vector3 crossProduct(vector3 u, vector3 v);
        double length();
        vector3 unit(vector3 u);
        vector3 scale(double s, vector3 u);
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
