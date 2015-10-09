#ifndef LIGHT_H
#define LIGHT_H

#include "rgb.h"
#include "vector3.h"

class light
{
    public:
        light();
        light(vector3 loc, bool isPnt, rgb color);
        bool getIsPnt();
        vector3 getLoc();
        rgb getColor();
        void setIsPnt(bool isPnt);
        void setLoc(vector3 loc);
        void setColor(rgb color);
    private:
        vector3 loc;
        bool isPnt;
        rgb color;
};

#endif // LIGHT_H
