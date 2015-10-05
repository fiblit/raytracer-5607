#include "cylinder.h"

cylinder::cylinder() { }

cylinder::cylinder(double u, double v, double radius, double minw, double maxw, cylTypes type, material mtl)
{
    this->u = u;
    this->v = v;
    this->minw = minw;
    this->maxw = maxw;
    this->radius = radius;
    this->type = type;
    this->mtl = mtl;
}

bool cylinder::intersect(ray rr, double &t)
{
    point rloc = rr.getLoc();
    vector3 rdir = rr.getDir();
    double a;
    double b;
    double c;

    double endMinInter;//the t for rr intersecting the w=wmin plane
    double endMinInterU;//the u position of where rr intersects the w=wmin plane
    double endMinInterV;//the v position ...
    double endMaxInter;//for max's plane
    double endMaxInterU;//for max's plane
    double endMaxInterV;//for max's plane
    bool endMinHits = true;//For whether rr is parrallel to the plane, and if it is outside the circle (later)
    bool endMaxHits = true;

    switch (getType())
    {
        case cylTypes::X://u = y, v = z, w = x
            a = pow(rdir.getY(), 2) + pow(rdir.getZ(), 2);
            b = 2 * (rdir.getY() * (rloc.getY() - getU()) + rdir.getZ() * (rloc.getZ() - getV()));
            c = pow(rloc.getY()-getU(), 2) + pow(rloc.getZ()-getV(), 2) - pow(radius, 2);

            if (rdir.getX() < 0)
            {
                endMinHits = false;
                endMaxHits = false;
            }
            else
            {
                endMinInter = (getMinw() - rloc.getX())/rdir.getX();//the t for the intersection with min plane
                endMaxInter = (getMaxw() - rloc.getX())/rdir.getX();//... max plane
                endMinInterU = rloc.getY() + rdir.getY()*endMinInter;
                endMinInterV = rloc.getZ() + rdir.getZ()*endMinInter;
                endMaxInterU = rloc.getY() + rdir.getY()*endMaxInter;
                endMaxInterV = rloc.getZ() + rdir.getZ()*endMaxInter;
            }
            break;
        case cylTypes::Y://u = x, v = z, w = y
            a = pow(rdir.getX(), 2) + pow(rdir.getZ(), 2);
            b = 2 * (rdir.getX() * (rloc.getX() - getU()) + rdir.getZ() * (rloc.getZ() - getV()));
            c = pow(rloc.getX()-getU(), 2) + pow(rloc.getZ()-getV(), 2) - pow(radius, 2);

            if (rdir.getY() < 0)
            {
                endMinHits = false;
                endMaxHits = false;
            }
            else
            {
                endMinInter = (getMinw() - rloc.getY())/rdir.getY();//the t for the intersection with min plane
                endMaxInter = (getMaxw() - rloc.getY())/rdir.getY();//... max plane
                endMinInterU = rloc.getX() + rdir.getX()*endMinInter;
                endMinInterV = rloc.getZ() + rdir.getZ()*endMinInter;
                endMaxInterU = rloc.getX() + rdir.getX()*endMaxInter;
                endMaxInterV = rloc.getZ() + rdir.getZ()*endMaxInter;
            }
            break;
        case cylTypes::Z://u = x, v = y, w = z
            a = pow(rdir.getX(), 2) + pow(rdir.getY(), 2);
            b = 2 * (rdir.getX() * (rloc.getX() - getU()) + rdir.getY() * (rloc.getY() - getV()));
            c = pow(rloc.getX()-getU(), 2) + pow(rloc.getY()-getV(), 2) - pow(radius, 2);

            if (rdir.getZ() < 0)
            {
                endMinHits = false;
                endMaxHits = false;
            }
            else
            {
                endMinInter = (getMinw() - rloc.getZ())/rdir.getZ();//the t for the intersection with min plane
                endMaxInter = (getMaxw() - rloc.getZ())/rdir.getZ();//... max plane
                endMinInterU = rloc.getX() + rdir.getX()*endMinInter;
                endMinInterV = rloc.getY() + rdir.getY()*endMinInter;
                endMaxInterU = rloc.getX() + rdir.getX()*endMaxInter;
                endMaxInterV = rloc.getY() + rdir.getY()*endMaxInter;
            }
            break;
    }

    double discrim = pow(b, 2) - (4 * a * c);//again assuming a=1

    bool sol1Hits = true;
    bool sol2Hits = true;

    if (discrim < 0.0)
    {
        sol1Hits = false;
        sol2Hits = false;
    }
    double sol1 = ((-b) + sqrt(discrim))/(2 * a);
    double sol2 = ((-b) - sqrt(discrim))/(2 * a);

    double winter1;//uh... w intersection point... it just happened to be winter.
    double winter2;//for sol2
    switch (getType())
    {
        case cylTypes::X:
            winter1 = rloc.getX() + rdir.getX()*sol1;
            winter2 = rloc.getX() + rdir.getX()*sol2;
            break;
        case cylTypes::Y:
            winter1 = rloc.getY() + rdir.getY()*sol1;
            winter2 = rloc.getY() + rdir.getY()*sol2;
            break;
        case cylTypes::Z:
            winter1 = rloc.getZ() + rdir.getZ()*sol1;
            winter2 = rloc.getZ() + rdir.getZ()*sol2;
            break;
    }

    if (sol1 < 0 || winter1 < minw || winter1 > maxw)
        sol1Hits = false;
    if (sol2 < 0 || winter2 < minw || winter2 > maxw)
        sol2Hits = false;
    if (sqrt(pow(endMinInterU - getU(),2) + pow(endMinInterV - getV(),2)) > getRadius())
        endMinHits = false;
    if (sqrt(pow(endMaxInterU - getU(),2) + pow(endMaxInterV - getV(),2)) > getRadius())
        endMaxHits = false;

    t = numeric_limits<double>::infinity();
    if (!(sol1Hits || sol2Hits || endMinHits || endMaxHits))
        return false;
    if (sol1Hits)
        t = sol1;
    if (sol2Hits)
        t = (sol2 < t)?sol2:t;
    if (endMinHits)
        t = (endMinInter < t)?endMinInter:t;
    if (endMaxHits)
        t = (endMaxInter < t)?endMaxInter:t;

    return true; //intersection at t
}

rgb cylinder::shadeRay(ray rr, double t, vector<light> lights, vector<object*> objects)
{
    /*
    I_l = ka*Od_l + Sum_i=1_nlights [Ip_i_l * sh * [kd*Od_l (N dot L_i) + ks * Os_l (N dot H_i)^n]]
    */
    rgb color = mtl.getOd() * mtl.getka();
    point inter = rr.getLoc() + rr.getDir() * t;
    point cen;
    if (getType() == cylTypes::X)
        cen = point(inter.getX(), getU(), getV());
    else if (getType() == cylTypes::Y)
        cen = point(getU(), inter.getY(), getV());
    else
        cen = point(getU(), getV(), inter.getZ());
    vector3 n = (inter.subtract(cen)).fscale(radius);
    vector3 v = rr.getDir() * (-1);//TO the viewer
    //cout << "rr: " << rr.getDir().getX() << " : " << rr.getDir().getY() << " : " << rr.getDir().getZ() << endl;
    for (light lit : lights)
    {
        vector3 l;
        if (lit.getIsDir())
            l = lit.getLoc() * (-1);//TO the light (right?)
        else
            l = lit.getLoc().toPoint().subtract(inter);
        l = l.unit();
        vector3 h = l + v;
        h = h.unit();

        int shadow = 1;
        ray shadowrr(inter, l);
        for(object* obj: objects)//for each sphere (object) in scene
        {
            double tlig;
            if(obj->intersect(shadowrr, tlig))//if path to light interesects some sphere
            {
                if(lit.getIsDir())
                {
                    if(tlig > 0.000001) // in front of me for directional
                    {
                        shadow = 0;//then in shadow
                        break;
                    }
                }
                else
                {
                    if (tlig > 0.000001 && (tlig <= lit.getLoc().toPoint().subtract(inter).length())) //or between us for point
                    {
                        shadow = 0;//then in shadow
                        break;
                    }
                }
            }
        }

        color = color + lit.getColor() * ((mtl.getOd() * (mtl.getkd() * max(0.0, n.dotProduct(l)))) + (mtl.getOs() * (mtl.getks() * pow(max(0.0, n.dotProduct(h)), mtl.getn())))) * shadow;
        //parenthesis are right     10   12         32   3         43      4                 5 4321   2         32   3         43      4   5                 6 54          543210
        if (color.getR() > 1.0)
            color.setR(1.0);
        if (color.getG() > 1.0)
            color.setG(1.0);
        if (color.getB() > 1.0)
            color.setB(1.0);
        //cout << "\nlight";
        //cout << "n: " << n.getX() << " : " << n.getY() << " : " << n.getZ() << endl;
        //cout << "v: " << v.getX() << " : " << v.getY() << " : " << v.getZ() << endl;
        //cout << "l: " << l.getX() << " : " << l.getY() << " : " << l.getZ() << endl;
        //cout << "h: " << h.getX() << " : " << h.getY() << " : " << h.getZ() << endl;
        //rgb d = (mtl.getOd() * (mtl.getkd() * max(0.0, n.dotProduct(l))));
        //rgb s = (mtl.getOs() * (mtl.getks() * pow(max(0.0, n.dotProduct(h)), mtl.getn())));
        //cout << "d: " << d.getR() << " : " << d.getG() << " : " << d.getB() << endl;
        //cout << "s: " << s.getR() << " : " << s.getG() << " : " << s.getB() << endl;
    }

    return color;
}

double cylinder::getRadius()
{
    return radius;
}

double cylinder::getU()
{
    return u;
}

double cylinder::getV()
{
    return v;
}

double cylinder::getMaxw()
{
    return maxw;
}

double cylinder::getMinw()
{
    return minw;
}

cylinder::cylTypes cylinder::getType()
{
    return type;
}

material cylinder::getMtl()
{
    return mtl;
}

void cylinder::setRadius(double r)
{
    radius = r;
}

void cylinder::setU(double u)
{
    this->u = u;
}

void cylinder::setV(double v)
{
    this->v = v;
}

void cylinder::setMaxw(double maxw)
{
    this->maxw = maxw;
}

void cylinder::setMinw(double minw)
{
    this->minw = minw;
}

void cylinder::setType(cylTypes type)
{
    this->type = type;
}

void cylinder::setMtl(material mtl)
{
    this->mtl = mtl;
}
