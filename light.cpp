#include "light.h"

light::light() { }

light::light(vector3 loc, bool isDir, rgb color)
{
    this->loc = loc;
    this->isDir = isDir;
    this->color = color;
}

bool light::getIsDir()
{
    return isDir;
}

vector3 light::getLoc()
{
    return loc;
}

rgb light::getColor()
{
    return color;
}

void light::setIsDir(bool isDir)
{
    this->isDir = isDir;
}

void light::setLoc(vector3 loc)
{
    this->loc = loc;
}

void light::setColor(rgb color)
{
    this->color = color;
}
