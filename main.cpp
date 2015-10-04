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
#include "rgb.h"
#include "ray.h"
#include "IO.h"

using namespace std;

double const PI = 3.14159265358979323846264338327950288;

int main(int argc, char *argv[])
{
    //GetInFileName
    string fileName;//name of input file
    ifstream inFile;//input file
	if (argc > 2)
		return errMsg(INVCALL, (string)"Usage: \'" + (string)argv[0] + (string)" filename\', or \'" + (string)argv[0] + (string)"\' and it will prompt.");
	else if (argc == 2)
		fileName = argv[1];
	else
	{
		cout << "Please type the name of an existing .txt file (please include extension): ";
		getline(cin,fileName);
	}

    //Validate inFile
    inFile.open(fileName);
    if (!inFile.is_open())
        return errMsg(INVFILE,"Please enter the name of an existing .txt file.");
    if (fileName.length()<=4 || fileName.substr(fileName.length()-4,fileName.length()-1)!=".txt")//the length <=4 is to make sure substr can be run in bounds.
    {
        inFile.close();
        return errMsg(INVFILE,"Please enter a .txt file");
    }

    //Read Scene
    int errval;
    point eye;
    vector3 viewdir, updir;
    int imgWidth, imgHeight;
    double fovh;
    rgb bkgcolor;
    vector<object *> objects;//Might be later changed to "objects"
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
    fd.objects = &objects;
    fd.lights= &lights;
    if ((errval = getInFileData(inFile, fd)))
        return errval;

    /*
    //Scene Debug
    cout << "width: " << imgWidth << "\n";
    cout << "height: " << imgHeight << "\n";
    cout << "eye: " << eye.getX() << " : " << eye.getY() << " : " << eye.getZ() << "\n";
    cout << "viewdir: " << viewdir.getX() << " : " << viewdir.getY() << " : " << viewdir.getZ() << "\n";
    cout << "updir: " << updir.getX() << " : " << updir.getY() << " : " << updir.getZ() << "\n";
    cout << "fovh: " << fovh << "\n";
    cout << "bkgcolor: " << bkgcolor.getR() << " : " << bkgcolor.getG() << " : " << bkgcolor.getB() << "\n";
    for(sphere s : objects)
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
    //imgBuf is still a 2D *image*, it is just a 1D array. This was more for my convenience than anything else.
    //I'll fix it in hw 1b (If I need to) once I understand the syntax of dynamically allocating multi-dimensional arrays in C++.
    rgb *imgBuf = new rgb[imgHeight*imgWidth];//apparently allocating multidimensional arrays is frustrating(confusing,strange,stupid).
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
    point ul = (eye.vect() + nviewdir + v.scale(viewHeight/2) - u.scale(viewWidth/2)).toPoint();
    point ur = (eye.vect() + nviewdir + v.scale(viewHeight/2) + u.scale(viewWidth/2)).toPoint();
    point ll = (eye.vect() + nviewdir - v.scale(viewHeight/2) - u.scale(viewWidth/2)).toPoint();
    point lr = (eye.vect() + nviewdir - v.scale(viewHeight/2) + u.scale(viewWidth/2)).toPoint();//unused
    vector3 deltav = ll.subtract(ul).fscale(imgHeight-1);
    vector3 deltah = ur.subtract(ul).fscale(imgWidth-1);

    //Raycast/trace loop
    for(int y = 0; y < imgHeight; y++)//for each pixel in image
    {
        for(int x = 0; x < imgWidth; x++)
        {
            imgBuf[y*imgWidth+x] = bkgcolor;//initialize to background
            int closest = -1;
            double closestInter = numeric_limits<double>::infinity();
            ray curRay (eye, (ul.vect() + deltah.scale(x) + deltav.scale(y) - eye.vect()).unit());
            for(int i = 0; i < objects.size(); i++)//for each sphere (object) in scene
            {
                double t;
                if(objects[i]->intersect(curRay,t) && (closestInter > t))//returns true if intersected, assigns closer (non-neg) intersection to t
                {
                    closestInter = t;
                    closest = i;
                }
            }
            if (closest!=-1)
                imgBuf[y*imgWidth+x] = objects[closest]->shadeRay(curRay, closestInter, lights, objects);
        }
    }

    writeOutFile(fileName, imgBuf, imgWidth, imgHeight);
    delete[] imgBuf;

    return 0;
}
