#include "triangle.h"

triangle::triangle(material mtl)
{
    for (int i = 0; i < 3; i++)
    {
        v[i] = -1;
        vt[i] = -1;
        vn[i] = -1;
    }
    this->mtl = mtl;
}

triangle::triangle(int v[3], material mtl)
{
    for (int i = 0; i < 3; i++)//deepcopy
    {
        this->v[i] = v[i];
        vt[i] = -1;
        vn[i] = -1;
    }
    this->mtl = mtl;
}

triangle::triangle(int v[3], int vn[3], material mtl)
{
    for (int i = 0; i < 3; i++)
    {
        this->v[i] = v[i];
        vt[i] = -1;
        this->vn[i] = vn[i];
    }
    this->mtl = mtl;
}

triangle::triangle(int v[3], int vt[3], int vn[3], material mtl)
{
    for (int i = 0; i < 3; i++)
    {
        this->v[i] = v[i];
        this->vt[i] = vt[i];
        this->vn[i] = vn[i];
    }
    this->mtl = mtl;
}

bool triangle::intersect(ray rr, double &t, fileData_t *fd)//fd for vertices
{
    //for convenience
    point v0 = (*(fd->vertices))[v[0]];
    point v1 = (*(fd->vertices))[v[1]];
    point v2 = (*(fd->vertices))[v[2]];
    vector3 dir = rr.getDir();
    point loc = rr.getLoc();

    //intersection (calculate Beta/Gamma/t), follows formulation from Book
    double a = v0.getX() - v1.getX();
    double b = v0.getY() - v1.getY();
    double c = v0.getZ() - v1.getZ();
    double d = v0.getX() - v2.getX();
    double e = v0.getY() - v2.getY();
    double f = v0.getZ() - v2.getZ();
    double g = dir.getX();
    double h = dir.getY();
    double i = dir.getZ();
    double j = v0.getX() - loc.getX();
    double k = v0.getY() - loc.getY();
    double l = v0.getZ() - loc.getZ();
    double m = e*i - h*f;
    double n = g*f - d*i;
    double o = d*h - e*g;
    double p = a*k - j*b;
    double q = j*c - a*l;
    double r = b*l - k*c;
    double s = a*m + b*n + c*o;

    t = -(f*p + e*q + d*r)/s;
    if (t <= 0)
        return false;
    gamma = (i*p + h*q + g*r)/s;
    if (!(gamma > 0 && gamma < 1))
        return false;
    beta = (j*m + k*n + l*o)/s;//beta and gamma are defined as member variables in triangle.
    if (!(beta > 0 && beta < 1-gamma))
        return false;
    /*//ray/plane intersection
    vector3 e1 = v1.subtract(v0);
    vector3 e2 = v2.subtract(v0));
    vector3 n = e1.crossProduct(e2);
    double a = n.getX();
    double b = n.getY();
    double c = n.getZ();
    double d = -(a*v0.getX() + b*v0.getY() + c*v0.getZ());
    double denom = (a*dir.getX() + b*dir.getY() + c*dir.getZ())
    if (denom == 0)
        return false; //ray is parallel to plane
    t = -(a*loc.getX() + b*loc.getY() + c*loc.getZ() + d)/denom;
    if (t < 0)
        return false;//behind me :c

    //point in polygon
    calculate A
    calculate areaA
    calculate areaB
    calculate areaC
    calculate alpha
    caluclate beta
    calculate gamma
    validation check (alpha+beta+gamma == 1)
    */
    return true;
}

rgb triangle::shadeRay(ray rr, double t, fileData_t *fd)//fd for lights, objects, textures, vertices, vertex texture coords, and vertex normals.
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
