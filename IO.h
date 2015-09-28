#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "point.h"
#include "sphere.h"
#include "vector3.h"
#include "rgb.h"

using namespace std;

string getWord(string& line);
bool isInt(string str);
bool isDouble(string str);
int errMsg(int err, string msg);
enum errNum { INVCALL=1, INVFILE, INVKWD, MSSKWD, MSSPRM, INVPRM, REPKWD }; //For the errMsg function
typedef struct fileData
{
    int *width;
    int *height;
    point *eye;
    vector3 *viewdir;
    vector3 *updir;
    double *fovh;
    rgb *bkgcolor;
    vector<sphere> *spheres;
} fileData_t;
int getInFileData(ifstream &inFile, fileData_t fd);
int* getIntParams(int n, string line);//helper function for above
double* getDoubleParams(int n, string line);//helper for above
void writeOutFile(string fileName, rgb *imgBuf, int imgWidth, int imgHeight);

#endif // IO_H
