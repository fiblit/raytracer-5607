#ifndef TRACERAY_H
#define TRACERAY_H

#include <limits> //for infinity

#include "rgb.h"
#include "ray.h"
#include "object.h"
#include "IO.h" //for filedata :p
#include "constants.h"
#include "vector3.h"
#include "point.h"
#include "light.h"

void traceRay(ray rr, fileData_t *fd, rgb &color, int depth);
//What follows are essentially helper functions in any object::shadeRay function
bool surfaceBack(vector3 &n, vector3 v);
void shadeForEachLight(vector3 n, vector3 v, point inter, rgb diffuse, material mtl, fileData_t *fd, rgb &color);
double calculateShadowValue(point inter, vector3 l, fileData_t *fd, light lit);
void shadeForTraces(vector3 n, vector3 v, point inter, material mtl, rgb &color, fileData_t* fd, int depth, bool backside);
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
    int depth);
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
    int depth);

#endif // TRACERAY_H
