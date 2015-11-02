#include "traceRay.h"

#include "stdio.h"

/* find the color of a ray */
void traceRay(ray rr, fileData_t *fd, rgb &color, int depth)
{
    if (depth > MAXRECURSIONDEPTH)
        return;

    vector<object *> objects = *(fd->objects);
    int closest = -1;
    double closestInter = numeric_limits<double>::infinity();
    for(int i = 0; i < (int)objects.size(); i++)//for each object in scene
    {
        double t;
        if(objects[i]->intersect(rr, t, fd) && (closestInter > t) && (t > EPSILON))//returns true if intersected, assigns closer (non-neg) intersection to t
        {
            closestInter = t;
            closest = i;
        }
    }
    if (closest!=-1)
        color = (objects[closest]->shadeRay(rr, closestInter, fd, depth+1));//I'm pretty sure this doesn't work, if not, I can malloc here
}

/* calculate diffuse and specular components of the color. These are the light based components of the phong model */
void shadeForEachLight(vector3 n, vector3 v, point inter, rgb diffuse, material mtl, fileData_t *fd, rgb &color)
{
    for (light lit : *(fd->lights))
    {
        if (*(fd->softShadow))
        {
            rgb clrs[RAYBUNDLESIZE] = {rgb(0, 0, 0)};
            rgb *colors = clrs;
            for (int i = 0; i < RAYBUNDLESIZE; i++)
            {
                vector3 l;
                point litLoc = lit.getLoc().toPoint().jitter(0.75);
                if (lit.getIsPnt())
                    l = litLoc.subtract(inter);
                else
                    l = litLoc.vect() * (-1);//TO the light (right?)
                l = l.unit();
                vector3 h = l + v;
                h = h.unit();

                double shadow = calculateShadowValue(inter, l, fd, lit);

                rgb dcolor = diffuse * (mtl.getkd() * max(0.0, n.dotProduct(l)));
                rgb scolor = mtl.getOs() * (mtl.getks() * pow(max(0.0, n.dotProduct(h)), mtl.getn()));
                colors[i] = lit.getColor() * (dcolor + scolor) * shadow;
            }

            color = color + color.average(colors, RAYBUNDLESIZE);
        }
        else
        {
            vector3 l;
            if (lit.getIsPnt())
                l = lit.getLoc().toPoint().subtract(inter);
            else
                l = lit.getLoc() * (-1);//TO the light (right?)
            l = l.unit();
            vector3 h = l + v;
            h = h.unit();

            double shadow = calculateShadowValue(inter, l, fd, lit);

            rgb dcolor = diffuse * (mtl.getkd() * max(0.0, n.dotProduct(l)));
            rgb scolor = mtl.getOs() * (mtl.getks() * pow(max(0.0, n.dotProduct(h)), mtl.getn()));
            color = color + lit.getColor() * (dcolor + scolor) * shadow;
        }
        //Check for overflow
        color = color.clamp();
    }
}

/* calculate shadow value */
double calculateShadowValue(point inter, vector3 l, fileData_t *fd, light lit)
{
    double shadow = 1;
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
                    shadow *= (1 - obj->getMtl().getOpacity());
                }
            }
            else
            {
                if(tlig > 0) // in front of me for directional
                {
                    shadow *= (1 - obj->getMtl().getOpacity());
                }
            }
        }
    }
    return shadow;
}


/* calculate transmitted and reflected component of color */
void shadeForTraces(vector3 n, vector3 v, point inter, material mtl, rgb &color, fileData_t* fd, int depth)
{
    if (mtl.getEta() != -1 && mtl.getOpacity() != -1)//eta and opacity have been defined
    {
        double etaIncident = 1.0; //Will vary when object stack is implemented ( stack.top().getMtl().getEta() )
        double etaTransmit = mtl.getEta(); //Will vary for stack mtl.getEta(), but later when n is flipped, the stack top will have to be flipped? (Unless it also holds in/out?)
        double cosIncident = v.dotProduct(n);
        if (cosIncident < 0)//Backside of surface
        {
            n = n * -1;
            cosIncident = v.dotProduct(n);//recalculate cosIncident
            double t = etaIncident;//swap incident and transmit as we are on the inside of the surface, NOTE/TODO: This won't work for stack
            etaIncident = etaTransmit;
            etaTransmit = t;
        }
        vector3 incident = v; //for convenience
        double factor_0 = pow(((etaTransmit - etaIncident)/(etaTransmit + etaIncident)), 2);
        double factor_r = factor_0 + (1 - factor_0)*pow(1 - cosIncident, 5);//schlick approximation of fresnel reflectance

        // add color component from reflections
        shadeForReflection(etaIncident, etaTransmit, n, cosIncident, inter, incident, factor_r, color, fd, depth);

        // add color component from transmissions
        shadeForTransmission(etaIncident, etaTransmit, n, cosIncident, inter, incident, factor_r, mtl, color, fd, depth);
    }
}

/* calculate reflective component of color */
void shadeForReflection(
    double etaIncident,
    double etaTransmit,
    vector3 n,
    double cosIncident,
    point inter,
    vector3 incident,
    double factor_r,
    rgb &color,
    fileData_t *fd,
    int depth)
{
    //calculate reflected ray
    vector3 reflected = (n * 2 * cosIncident) - incident;
    ray reflecRay = ray(inter + reflected*EPSILON, reflected);

    //Get color for reflected ray
    rgb rcolor = rgb(0, 0, 0);
    traceRay(reflecRay, fd, rcolor, depth);
    color = color + rcolor * factor_r;

    //Check for overflow
    color = color.clamp();
}

/* calculate transmitted component of color */
void shadeForTransmission(
    double etaIncident,
    double etaTransmit,
    vector3 n,
    double cosIncident,
    point inter,
    vector3 incident,
    double factor_r,
    material mtl,
    rgb &color,
    fileData_t *fd,
    int depth)
{
    //calculate transmitted ray
    double sinIncident = sqrt(1 - pow(cosIncident, 2));
    double etaRatio = etaIncident/etaTransmit;
    if (sinIncident <= etaTransmit/etaIncident && mtl.getOpacity() != 1)//check for TIR and unneccesary computation
    {
        vector3 transmitted = (n * -1)
            * sqrt(1 - (pow(etaRatio, 2) * (1 - pow(cosIncident, 2))))
            + (n * cosIncident - incident) * etaRatio;
        ray transmitRay(inter + transmitted * EPSILON, transmitted);

        //get color for transmitted ray
        rgb tcolor = rgb(0, 0, 0);
        traceRay(transmitRay, fd, tcolor, depth);
        color = color + tcolor * (1 - factor_r) * (1 - mtl.getOpacity());
    }

    //Check for overflow
    color = color.clamp();
}
