#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <vector>

//#include "vector3.h"
//#include "point.h"
#include "sphere.h"
#include "cylinder.h"
#include "rgb.h"
#include "ray.h"
#include "IO.h"
#include "texture.h"
#include "triangle.h"
#include "traceRay.h"
#include "constants.h"

using namespace std;

int main(int argc, char *argv[])
{
    //GetInFileName
    string inFileName;
    string outFileName;
    ifstream inFile;//input file
	if (argc > 3)
		return errMsg(INVCALL, (string)"Usage: \'" + (string)argv[0] + (string)" inputfile outputfile\', or \'" +
            (string)argv[0] + (string)" inputfile\', or \'" +
            (string)argv[0] + (string)"\' and it will prompt for files.");
	else if (argc == 3)
	{
        inFileName = argv[1];
        outFileName = argv[2];
    }
    else if (argc == 2)
    {
        inFileName = argv[1];
        outFileName = "";
    }
	else
	{
		cout << "Please type the name of an existing .txt file (please include extension): ";
		getline(cin, inFileName);
		cout << "Output File? (please include ppm extension) Leave blank if you don't care, it will save to the local directory: ";
		getline(cin, outFileName);
	}

    //Validate Infile
    inFile.open(inFileName);
    if (!inFile.is_open())
        return errMsg(INVFILE,"Please enter the name of an existing .txt file for the input.");
    if (inFileName.length()<=4 || inFileName.substr(inFileName.length()-4,inFileName.length()-1)!=".txt")//the length <=4 is to make sure substr can be run in bounds.
    {
        inFile.close();
        return errMsg(INVFILE,"Please enter the name of an existing .txt file for the input.");
    }

    //Validate OutFileName
    if (outFileName != "" && (outFileName.length()<=4 || outFileName.substr(outFileName.length()-4, outFileName.length()-1)!=".ppm"))
        return errMsg(INVFILE, "Please enter with a .ppm exetension for the output");


    //Read Scene
    int errval;
    point eye;
    vector3 viewdir, updir;
    int imgWidth, imgHeight;
    double fovh;
    rgb bkgcolor;
    bool parallel;
    vector<object *> objects;//The pointer is because object is an ABC (you can't instantiate an object)
    vector<light> lights;
    vector<texture> textures;
    vector<point> vertices;
    vector<textureCoord> vTextures;
    vector<vector3> vNormals;
    double viewDist;
    bool softShadow;
    //init fileData
    fileData_t fd;
    fd.eye = &eye;
    fd.viewdir = &viewdir;
    fd.updir = &updir;
    fd.width = &imgWidth;
    fd.height = &imgHeight;
    fd.fovh = &fovh;
    fd.bkgcolor = &bkgcolor;
    fd.parallel = &parallel;
    fd.objects = &objects;
    fd.lights = &lights;
    fd.textures = &textures;
    fd.vertices = &vertices;
    fd.vTextures = &vTextures;
    fd.vNormals = &vNormals;
    fd.viewDist = &viewDist;
    fd.softShadow = &softShadow;
    if ((errval = getInFileData(inFile, fd)))
        return errval;
    inFile.close();

    //Image Computations
    rgb **imgBuf = new rgb*[imgHeight];
    for (int i = 0; i < imgHeight; i++)
        imgBuf[i] = new rgb[imgWidth];

    double aspect = ((double)imgWidth)/imgHeight;//cast to double because they are ints

    //Viewing Window Compuatations
    vector3 u = viewdir.crossProduct(updir); //Find the vector horizontal to the window
    if (abs(u.length() - EPSILON) < 0)//viewdir and updir were parallel
        return errMsg(INVPRM, "viewdir and updir are parallel. Please offset updir");
    u = u.unit();
    vector3 nviewdir = viewdir.unit();
    vector3 v = u.crossProduct(nviewdir); //Find the vector vertical to the window //v is unit length due to above line
    double viewWidth = 2*viewDist*tan(fovh/2*PI/180);
    double viewHeight = viewWidth/aspect;
    point ul = (eye.vect() + v.scale(viewHeight/2) - u.scale(viewWidth/2)).toPoint();
    point ur = (eye.vect() + v.scale(viewHeight/2) + u.scale(viewWidth/2)).toPoint();
    point ll = (eye.vect() - v.scale(viewHeight/2) - u.scale(viewWidth/2)).toPoint();
    point lr = (eye.vect() - v.scale(viewHeight/2) + u.scale(viewWidth/2)).toPoint();//unused
    if (!parallel)
    {
        if (viewDist != -1)
        {
            ul = ul + nviewdir * viewDist;
            ur = ur + nviewdir * viewDist;
            ll = ll + nviewdir * viewDist;
            lr = lr + nviewdir * viewDist;
        }
        else
        {
            ul = ul + nviewdir;
            ur = ur + nviewdir;
            ll = ll + nviewdir;
            lr = lr + nviewdir;
        }
    }
    vector3 deltav = ll.subtract(ul).fscale(imgHeight-1);
    vector3 deltah = ur.subtract(ul).fscale(imgWidth-1);

    //Raycast/trace loop

    if (viewDist != -1)//Do depth of field bundle
    {
        rgb ***imgBufBundle = new rgb**[imgHeight];//imgBuf's where each "pixel" is actually DOFBUNDLESIZE # of pixels
        for (int i = 0; i < imgHeight; i++)
        {
            imgBufBundle[i] = new rgb*[imgWidth];
            for (int j = 0; j < imgWidth; j++)
                imgBufBundle[i][j] = new rgb[DOFBUNDLESIZE];
        }

        for (int i = 0; i<DOFBUNDLESIZE; i++)//trace out RAYBUNDLESIZE rays, taking the average result
        {
            point newEye = eye.jitter(0.75);
            for(int y = 0; y < imgHeight; y++)//for each pixel in image
            {
                for(int x = 0; x < imgWidth; x++)
                {
                    ray curRay;
                    if (parallel)
                        curRay = ray((ul.vect() + deltah.scale(x) + deltav.scale(y)).toPoint(), nviewdir);//nviewdir is just viewdir.unit()
                    else
                        curRay = ray(newEye, (ul.vect() + deltah.scale(x) + deltav.scale(y) - newEye.vect()).unit());
                    imgBufBundle[y][x][i] = bkgcolor;//initialize to background
                    traceRay(curRay, &fd, imgBufBundle[y][x][i], 0);
                }
            }
        }

        for (int i = 0; i < imgHeight; i++)
            for (int j = 0; j < imgWidth; j++)
                imgBuf[i][j] = imgBuf[i][j].average(imgBufBundle[i][j], DOFBUNDLESIZE);

        for (int i = 0; i < imgHeight; i++)
        {
            for (int j = 0; j < imgWidth; j++)
                delete[] imgBufBundle[i][j];
            delete[] imgBufBundle[i];
        }
        delete[] imgBufBundle;
    }
    else
    {
        for(int y = 0; y < imgHeight; y++)//for each pixel in image
        {
            for(int x = 0; x < imgWidth; x++)
            {
                ray curRay;
                if (parallel)
                    curRay = ray((ul.vect() + deltah.scale(x) + deltav.scale(y)).toPoint(), nviewdir);//nviewdir is just viewdir.unit()
                else
                    curRay = ray(eye, (ul.vect() + deltah.scale(x) + deltav.scale(y) - eye.vect()).unit());
                imgBuf[y][x] = bkgcolor;//initialize to background
                traceRay(curRay, &fd, imgBuf[y][x], 0);
            }
        }
    }

    for (object * obj : objects)
        delete obj;
    for (texture tex : textures)
    {
        for (int i = 0; i < tex.getHeight(); i++)
            delete[] tex.getImg()[i];
        delete[] tex.getImg();
    }

    if (outFileName == "")//unspecified, save as same name in local directory
    {
        outFileName = inFileName.replace(inFileName.length()-4,inFileName.length()-1,".ppm");//change extension from .txt to .ppm
        outFileName = "/" + outFileName; //For removing the path safely
        outFileName = outFileName.replace(0,outFileName.find_last_of('/')+1,"");//remove path
    }

    try
    {
        writeOutFile(outFileName, imgBuf, imgWidth, imgHeight);
    }
    catch(int e)
    {
        return e;
    }

    for (int i = 0; i < imgHeight; i++)
        delete[] imgBuf[i];
    delete[] imgBuf;

    return 0;
}
