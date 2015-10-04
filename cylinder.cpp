#include "cylinder.h"

cylinder::cylinder() { }

cylinder::cylinder(double radius, double u, double v, cylTypes type, material mtl)
{
    this->radius = radius;
    this->u = u;
    this->v = v;
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
    switch (getType())
    {
        case cylTypes::X://u = y, v = z
            a = pow(rdir.getY(), 2) + pow(rdir.getZ(), 2);
            b = 2 * (rdir.getY() * (rloc.getY() - getU()) + rdir.getZ() * (rloc.getZ() - getV()));
            c = pow(rloc.getY()-getU(), 2) + pow(rloc.getZ()-getV(), 2) - pow(radius, 2);
            break;
        case cylTypes::Y://u = x, v = z
            a = pow(rdir.getX(), 2) + pow(rdir.getZ(), 2);
            b = 2 * (rdir.getX() * (rloc.getX() - getU()) + rdir.getZ() * (rloc.getZ() - getV()));
            c = pow(rloc.getX()-getU(), 2) + pow(rloc.getZ()-getV(), 2) - pow(radius, 2);;
            break;
        case cylTypes::Z://u = x, v = y
            a = pow(rdir.getX(), 2) + pow(rdir.getY(), 2);
            b = 2 * (rdir.getX() * (rloc.getX() - getU()) + rdir.getY() * (rloc.getY() - getV()));
            c = pow(rloc.getX()-getU(), 2) + pow(rloc.getY()-getV(), 2) - pow(radius, 2);;
            break;
    }

    double discrim = pow(b, 2) - (4 * a * c);//again assuming a=1

    if (discrim < 0.0)
    {
        return false; //No intersection, ray missed
    }
    double sol1 = ((-b) + sqrt(discrim))/(2 * a);
    double sol2 = ((-b) - sqrt(discrim))/(2 * a);
    if (sol1 < 0)
    {
        if (sol2 < 0)
            return false; //No intersection, behind us
        t = sol2;
    }
    else if (sol2 < 0)
        t = sol1;
    else if (sol1 <= sol2)
        t = sol1;
    else
        t = sol2;

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

void cylinder::setType(cylTypes type)
{
    this->type = type;
}

void cylinder::setMtl(material mtl)
{
    this->mtl = mtl;
}
