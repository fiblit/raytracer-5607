#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <limits>
//#include "vector3.h"
//#include "point.h"
#include "sphere.h"
#include "cylinder.h"
#include "rgb.h"
#include "ray.h"
#include "IO.h"

using namespace std;

double const PI = 3.14159265358979323846264338327950288;//I actually know those digits by heart, I have it down to a little rhythm.

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
    vector<object *> objects;//The pointer is because object is an ABC
    vector<light> lights;
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
    fd.lights= &lights;
    if ((errval = getInFileData(inFile, fd)))
        return errval;
    inFile.close();
    /*
    //Scene Debug
    cout << "width: " << imgWidth << "\n";
    cout << "height: " << imgHeight << "\n";
    cout << "eye: " << eye.getX() << " : " << eye.getY() << " : " << eye.getZ() << "\n";
    cout << "viewdir: " << viewdir.getX() << " : " << viewdir.getY() << " : " << viewdir.getZ() << "\n";
    cout << "updir: " << updir.getX() << " : " << updir.getY() << " : " << updir.getZ() << "\n";
    cout << "fovh: " << fovh << "\n";
    cout << "bkgcolor: " << bkgcolor.getR() << " : " << bkgcolor.getG() << " : " << bkgcolor.getB() << "\n";
    for(sphere s : objects)//I don't think this works anymore.
    {
        cout << "sphere: " << s.getLoc().getX() << " : " << s.getLoc().getY() << " : " << s.getLoc().getZ() << " : "
            << s.getRadius() << " : "
            << s.getMtl().getOd().getR() << " : " << s.getMtl().getOd().getG() << " : " << s.getMtl().getOd().getB() << " : "
            << s.getMtl().getOs().getR() << " : " << s.getMtl().getOs().getG() << " : " << s.getMtl().getOs().getB() << " : "
            << s.getMtl().getka() << " : " << s.getMtl().getkd() << " : " << s.getMtl().getks() << " : "
            << s.getMtl().getn() << "\n";
    }
    for(light l : lights)
    {
        cout << "light: " << l.getLoc().getX() << " : " << l.getLoc().getY() << " : " << l.getLoc().getZ() << " : "
            << l.getIsDir() << " : " << l.getColor().getR() << " : " << l.getColor().getG() << " : " << l.getColor().getB() << "\n";
    }
    cout << endl;
    //end Scene Debug
    */

    //Image Computations
    rgb **imgBuf = new rgb*[imgHeight];
    for (int i = 0; i < imgHeight; i++)
        imgBuf[i] = new rgb[imgWidth];

    double aspect = ((double)imgWidth)/imgHeight;//cast to double because they are ints

    //Viewing Window Compuatations
    vector3 u = viewdir.crossProduct(updir); //Find the vector horizontal to the window
    if (u.length() == 0)//viewdir and updir were parallel
        return errMsg(INVPRM,"viewdir and updir are parallel. Please offset updir");
    u = u.unit();
    vector3 nviewdir = viewdir.unit();
    vector3 v = u.crossProduct(nviewdir); //Find the vector vertical to the window //v is unit length due to above line
    //Since it is arbitrary, focal depth or "d" is 1.0 for convenience. Therefore it is never written, since it is only used in multiplications!
    double viewWidth = 2*tan(fovh/2*PI/180);
    double viewHeight = viewWidth/aspect;
    point ul = (eye.vect() + nviewdir*((double)parallel) + v.scale(viewHeight/2) - u.scale(viewWidth/2)).toPoint();
    point ur = (eye.vect() + nviewdir*((double)parallel) + v.scale(viewHeight/2) + u.scale(viewWidth/2)).toPoint();
    point ll = (eye.vect() + nviewdir*((double)parallel) - v.scale(viewHeight/2) - u.scale(viewWidth/2)).toPoint();
    point lr = (eye.vect() + nviewdir*((double)parallel) - v.scale(viewHeight/2) + u.scale(viewWidth/2)).toPoint();//unused
    vector3 deltav = ll.subtract(ul).fscale(imgHeight-1);
    vector3 deltah = ur.subtract(ul).fscale(imgWidth-1);

    //Raycast/trace loop
    for(int y = 0; y < imgHeight; y++)//for each pixel in image
    {
        for(int x = 0; x < imgWidth; x++)
        {
            imgBuf[y][x] = bkgcolor;//initialize to background
            int closest = -1;
            double closestInter = numeric_limits<double>::infinity();
            ray curRay;
            if (parallel)
                curRay = ray((ul.vect() + deltah.scale(x) + deltav.scale(y)).toPoint(), nviewdir);//nviewdir is just viewdir.unit()
            else
                curRay = ray(eye, (ul.vect() + deltah.scale(x) + deltav.scale(y) - eye.vect()).unit());
            for(int i = 0; i < (int)objects.size(); i++)//for each sphere (object) in scene
            {
                double t;
                if(objects[i]->intersect(curRay,t) && (closestInter > t))//returns true if intersected, assigns closer (non-neg) intersection to t
                {
                    closestInter = t;
                    closest = i;
                }
            }
            if (closest!=-1)
                imgBuf[y][x] = objects[closest]->shadeRay(curRay, closestInter, lights, objects);
        }
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
