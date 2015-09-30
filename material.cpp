#include "material.h"

material::material() { }

material::material(rgb Od, rgb Os, double ka, double kd, double ks, int n)
{
    this->Od = Od;
    this->Os = Os;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->n = n;
}

rgb material::getOd()
{
    return Od;
}

rgb material::getOs()
{
    return Os;
}

double material::getka()
{
    return ka;
}

double material::getkd()
{
    return kd;
}

double material::getks()
{
    return ks;
}

int material::getn()
{
    return n;
}

void material::setOd(rgb Od)
{
    this->Od = Od;
}

void material::setOs(rgb Os)
{
    this->Os = Os;
}

void material::setka(double ka)
{
    this->ka = ka;
}

void material::setkd(double kd)
{
    this->kd = kd;
}

void material::setks(double ks)
{
    this->ks = ks;
}

void material::setn(int n)
{
    this->n = n;
}
