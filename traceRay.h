#ifndef TRACERAY_H
#define TRACERAY_H

#include <limits> //for infinity

#include "rgb.h"
#include "ray.h"
#include "object.h"
#include "IO.h" //for filedata :p

//This had to be pulled out of main due to dependency reasons (<object> -> Main, Main -> <object>)
void traceRay(ray rr, fileData_t *fd, rgb &color, int depth);

#endif // TRACERAY_H
