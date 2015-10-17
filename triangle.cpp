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
    /*
    I_l = ka*Od_l + Sum_i=1_nlights [Ip_i_l * sh * [kd*Od_l (N dot L_i) + ks * Os_l (N dot H_i)^n]]
    */
    point inter = rr.getLoc() + rr.getDir() * t;
    point v0 = (*(fd->vertices))[v[0]];
    point v1 = (*(fd->vertices))[v[1]];
    point v2 = (*(fd->vertices))[v[2]];
    vector3 e1 = v1.subtract(v0);
    vector3 e2 = v2.subtract(v0);
    vector3 n;
    double alpha = 1 - beta - gamma;

    if (vn[0] > -1)//vertex normals were defined. (If any aren't -1, they all aren't)
        n = (fd->vNormals->at(vn[0])) * alpha + (fd->vNormals->at(vn[1])) * beta + (fd->vNormals->at(vn[2])) * gamma;
    else
        n = e1.crossProduct(e2);
    n = n.unit();
    vector3 v = rr.getDir() * (-1);//TO the viewer

    rgb diffuse;
    if (vt[0] > -1)
    {
        double uTex = alpha * (fd->vTextures->at(vt[0])).u + beta * (fd->vTextures->at(vt[1])).u + gamma * (fd->vTextures->at(vt[2])).u;
        double vTex = alpha * (fd->vTextures->at(vt[0])).v + beta * (fd->vTextures->at(vt[1])).v + gamma * (fd->vTextures->at(vt[2])).v;
        diffuse = (*(fd->textures))[texIndex].getImg()[(int)(0.5 + vTex * ((*(fd->textures))[texIndex].getHeight() - 1))][(int)(0.5 + uTex * ((*(fd->textures))[texIndex].getWidth() - 1))];
    }
    else
        diffuse = mtl.getOd();
    rgb color = diffuse * mtl.getka();

    for (light lit : *(fd->lights))
    {
        vector3 l;
        if (lit.getIsPnt())
            l = lit.getLoc().toPoint().subtract(inter);
        else
            l = lit.getLoc() * (-1);//TO the light (right?)
        l = l.unit();
        vector3 h = l + v;
        h = h.unit();

        int shadow = 1;
        ray shadowrr(inter, l);
        for(object* obj: *(fd->objects))//for each sphere (object) in scene
        {
            double tlig;
            if(obj->intersect(shadowrr, tlig, fd))//if path to light interesects some sphere
            {
                if(lit.getIsPnt())
                {
                    if (tlig > 0.000001 && (tlig <= lit.getLoc().toPoint().subtract(inter).length())) //or between us for point
                    {
                        shadow = 0;//then in shadow
                        break;
                    }
                }
                else
                {
                    if(tlig > 0.000001) // in front of me for directional
                    {
                        shadow = 0;//then in shadow
                        break;
                    }
                }
            }
        }

        color = color + lit.getColor() * ((diffuse * (mtl.getkd() * max(0.0, n.dotProduct(l)))) + (mtl.getOs() * (mtl.getks() * pow(max(0.0, n.dotProduct(h)), mtl.getn())))) * shadow;
        //parenthesis are right     10   12         32   3         43      4                 5 4321   2         32   3         43      4   5                 6 54          543210
        if (color.getR() > 1.0)
            color.setR(1.0);
        if (color.getG() > 1.0)
            color.setG(1.0);
        if (color.getB() > 1.0)
            color.setB(1.0);
    }

    return color;
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
