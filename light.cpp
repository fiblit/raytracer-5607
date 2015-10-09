#include "light.h"

light::light() { }

light::light(vector3 loc, bool isPnt, rgb color)
{
    this->loc = loc;
    this->isPnt = isPnt;
    this->color = color;
}

bool light::getIsPnt()
{
    return isPnt;
}

vector3 light::getLoc()
{
    return loc;
}

rgb light::getColor()
{
    return color;
}

void light::setIsPnt(bool isPnt)
{
    this->isPnt = isPnt;
}

void light::setLoc(vector3 loc)
{
    this->loc = loc;
}

void light::setColor(rgb color)
{
    this->color = color;
}
