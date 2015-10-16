#include "triangle.h"

triangle::triangle() { }

triangle::triangle(int v[3])
{
    for (int i = 0; i < 3; i++)//deepcopy
    {
        this->v[i] = v[i];
    }
}

triangle::triangle(int v[3], int vn[3])
{
    for (int i = 0; i < 3; i++)
    {
        this->v[i] = v[i];
        this->vn[i] = vn[i];
    }
}

triangle::triangle(int v[3], int vt[3], int vn[3])
{
    for (int i = 0; i < 3; i++)
    {
        this->v[i] = v[i];
        this->vt[i] = vt[i];
        this->vn[i] = vn[i];
    }
}

bool triangle::intersect(ray rr, double &t)
{
    return false;
}

rgb triangle::shadeRay(ray rr, double t, vector<light> lights, vector<object*> objects, vector<texture> textures)
{
    return mtl.getOd();
}

int triangle::getv(int i)
{
    return v[i];
}

int triangle::getvt(int i)
{
    return vt[i];
}

int triangle::getvn(int i)
{
    return vn[i];
}

material triangle::getMtl()
{
    return mtl;
}

int triangle::getTexIndex()
{
    return texIndex;
}

void triangle::setv(int valv, int i)
{
    v[i] = valv;
}

void triangle::setvt(int valvt, int i)
{
    vt[i] = valvt;
}

void triangle::setvn(int valvn, int i)
{
    vn[i] = valvn;
}

void triangle::setMtl(material mtl)
{
    this->mtl = mtl;
}

void triangle::setTexIndex(int i)
{
    texIndex = i;
}
