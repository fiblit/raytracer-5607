#include "sphere.h"

sphere::sphere() {}

sphere::sphere(point loc, double radius, material mtl, int texIndex)
{
    this->loc = loc;
    this->radius = radius;
    this->mtl = mtl;
    this->texIndex = texIndex;
}

sphere::~sphere() { }

//Determines if the ray rr intersects this sphere.
//If so, it will put the closest parametric value, for the equation of rr, into t.
bool sphere::intersect(ray rr, double &t, fileData *fd)//fd for compatibility
{
    point rloc = rr.getLoc();
    vector3 rdir = rr.getDir();
    //Assume a = 1 because rdir is normalized.
    double b = 2 * (rdir.getX() * (rloc.getX() - loc.getX()) + rdir.getY() * (rloc.getY() - loc.getY()) + rdir.getZ() * (rloc.getZ() - loc.getZ()));
    double c = pow(rloc.getX()-loc.getX(),2) + pow(rloc.getY()-loc.getY(),2) + pow(rloc.getZ()-loc.getZ(),2) - pow(radius, 2);

    double discrim = pow(b, 2) - (4 * c);//again assuming a=1

    if (discrim < 0.0)
    {
        return false; //No intersection, ray missed
    }
    double sol1 = ((-b) + sqrt(discrim))/2;
    double sol2 = ((-b) - sqrt(discrim))/2;
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

rgb sphere::shadeRay(ray rr, double t, fileData *fd, int depth)//fd for lights, objects, textures
{
    /*
    I_l = ka*Od_l + Sum_i=1_nlights [Ip_i_l * sh * [kd*Od_l (N dot L_i) + ks * Os_l (N dot H_i)^n]] + reflected + transmitted
    */
    point inter = rr.getLoc() + rr.getDir() * t;
    vector3 n = (inter.subtract(loc)).fscale(radius);
    vector3 v = rr.getDir() * (-1);//TO the viewer

    double etaIncident = 1.0; //Will vary when object stack is implemented ( stack.top().getMtl().getEta() )
    double etaTransmit = mtl.getEta();
    //Flipping n code
    //flip etaTransmit etaIncident

    rgb diffuse;
    if (texIndex == -1)
        diffuse = mtl.getOd();
    else
    {
        double theta = atan2(n.getY(), n.getX());
        double phi = acos(n.getZ());
        if(theta < 0)
            theta += 2 * PI;
        double vTex = phi / PI;
        double uTex = theta / (2 * PI);

        diffuse = (*(fd->textures))[texIndex].getImg()[(int)(0.5 + vTex * ((*(fd->textures))[texIndex].getHeight() - 1))][(int)(0.5 + uTex * ((*(fd->textures))[texIndex].getWidth() - 1))];
    }
    rgb color = rgb(0, 0, 0);
    //cout << "rr: " << rr.getDir().getX() << " : " << rr.getDir().getY() << " : " << rr.getDir().getZ() << endl;

    //Calculate diffuse and specular components
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
        ray shadowrr(inter + l*EPSILON, l);
        for(object* obj: *(fd->objects))//for each sphere (object) in scene
        {
            double tlig;
            if(obj->intersect(shadowrr, tlig, fd))//if path to light interesects some sphere
            {
                if(lit.getIsPnt())
                {
                    if (tlig > 0 && (tlig <= lit.getLoc().toPoint().subtract(inter).length())) //or between us for point
                    {
                        shadow = 0;//then in shadow
                        break;
                    }
                }
                else
                {
                    if(tlig > 0) // in front of me for directional
                    {
                        shadow = 0;//then in shadow
                        break;
                    }
                }
            }
        }

        rgb dcolor = diffuse * (mtl.getkd() * max(0.0, n.dotProduct(l)));
        rgb scolor = mtl.getOs() * (mtl.getks() * pow(max(0.0, n.dotProduct(h)), mtl.getn()));
        color = color + lit.getColor() * (dcolor + scolor) * shadow;

        //Check for overflow
        if (color.getR() > 1.0)
            color.setR(1.0);
        if (color.getG() > 1.0)
            color.setG(1.0);
        if (color.getB() > 1.0)
            color.setB(1.0);
    }

    rgb acolor = diffuse * mtl.getka();
    color = color + acolor;

    if (mtl.getEta() != -1 && mtl.getOpacity() != -1)//eta and opacity have been defined
    {
        vector3 incident = v;

        //calculate reflected ray
        double cosIncident = max(0.0, v.dotProduct(n));
        vector3 reflected = (n * 2 * cosIncident) - incident;
        ray reflecRay = ray(inter, reflected);

        //Get color for reflected ray
        rgb rcolor = rgb(0, 0, 0);
        traceRay(reflecRay, fd, rcolor, depth);
        double factor_0 = pow(((etaTransmit - etaIncident)/(etaTransmit + etaIncident)), 2);
        double factor_r = factor_0 + (1 - factor_0)*pow(1 - cosIncident, 5);//schlick approximation of fresnel reflectance
        color = color + rcolor * factor_r;

        //transmitRay = ray();
        //rgb tcolor = rgb();
        //traceRay(transmitRay, fd, tcolor);
    }

    //Check for overflow
    if (color.getR() > 1.0)
        color.setR(1.0);
    if (color.getG() > 1.0)
        color.setG(1.0);
    if (color.getB() > 1.0)
        color.setB(1.0);

    return color;
}

point sphere::getLoc()
{
    return loc;
}

double sphere::getRadius()
{
    return radius;
}

material sphere::getMtl()
{
    return mtl;
}

int sphere::getTexIndex()
{
    return texIndex;
}

void sphere::setLoc(point p)
{
    loc = p;
}

void sphere::setRadius(double r)
{
    radius = r;
}

void sphere::setMtl(material mtl)
{
    this->mtl = mtl;
}

void sphere::setTexIndex(int i)
{
    texIndex = i;
}
