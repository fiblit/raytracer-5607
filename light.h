#ifndef LIGHT_H
#define LIGHT_H

#include "rgb.h"
#include "vector3.h"

class light
{
    public:
        light();
        light(vector3 loc, bool isDir, rgb color);
        bool getIsDir();
        vector3 getLoc();
        rgb getColor();
        void setIsDir(bool isDir);
        void setLoc(vector3 loc);
        void setColor(rgb color);
    private:
        vector3 loc;
        bool isDir;
        rgb color;
};

#endif // LIGHT_H
