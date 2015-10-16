#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "point.h"
#include "sphere.h"
#include "cylinder.h"
#include "vector3.h"
#include "rgb.h"
#include "light.h"
#include "texture.h"
#include "triangle.h"

using namespace std;

string getWord(string &line);
vector<string> split(string &str, string const delims);
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
    bool *parallel;
    rgb *bkgcolor;
    vector<object *> *objects;
    vector<light> *lights;
    vector<texture> *textures;
    vector<point> *vertices;
    vector<triangle::vertexTexture> *vTextures;
    vector<vector3> *vNormals;
    vector<triangle> *faces;
} fileData_t;
int getInFileData(ifstream &inFile, fileData_t fd);
int *getIntParams(int n, string &line);
double *getDoubleParams(int n, string &line);
string *getStringParams(int n, string &line);
texture getTexture(ifstream &texFile);
void writeOutFile(string fileName, rgb **imgBuf, int imgWidth, int imgHeight);

#endif // IO_H
