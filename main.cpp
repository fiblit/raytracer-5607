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

using namespace std;

string getWord(string& line);
bool isInt(string str);
bool isDouble(string str);
int errMsg(int err, string msg);
enum errNum { INVCALL=1, INVFILE, INVKWD, MSSKWD, MSSPRM, INVPRM, REPKWD }; //For the errMsg function
int getInFileData(ifstream &inFile,
    int &width,
    int &height,
    point &eye,
    vector3 &viewdir,
    vector3 &updir,
    double &fovh,
    rgb &bkgcolor,
    vector<sphere> &spheres);
int* getIntParams(int n, string line);//helper function for above
double* getDoubleParams(int n, string line);//helper for above
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
    vector<sphere> spheres;//Might be later changed to "objects"
    if ((errval = getInFileData(inFile, imgWidth, imgHeight, eye, viewdir, updir, fovh, bkgcolor, spheres)))
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
    for(sphere s : spheres)
    {
        cout << "sphere: " << s.getLoc().getX() << " : " << s.getLoc().getY() << " : " << s.getLoc().getZ() << " : "
            << s.getRadius() << " : " << s.getColor().getR() << " : " << s.getColor().getG() << " : " << s.getColor().getB() << "\n";
    }
    cout << endl;
    //end Scene Debug
    */

    //Image Computations
    rgb *imgBuf = new rgb[imgHeight*imgWidth];//apparently allocating multidimensional arrays is frustrating
    double aspect = imgWidth/imgHeight;

    //Viewing Window Compuatations
    vector3 u = viewdir.crossProduct(updir); //Find the vector horizontal to the window
    if (u.length() == 0)//viewdir and updir were parallel
        return errMsg(INVPRM,"viewdir and updir are parallel. Please offset updir");
    u = u.unit();
    vector3 nviewdir = viewdir.unit();
    vector3 v = u.crossProduct(nviewdir); //Find the vector vertical to the window //v is unit length due to above line
    //Since it is arbitrary, focal depth or "d" is 1.0 for convenience. Therefore it is never written!
    double viewWidth = 2*tan(fovh/2*PI/180);
    double viewHeight = viewWidth/aspect;
    point ul = (eye.vect() + nviewdir + v.scale(viewHeight/2) - u.scale(viewWidth/2)).toPoint();
    point ur = (eye.vect() + nviewdir + v.scale(viewHeight/2) + u.scale(viewWidth/2)).toPoint();
    point ll = (eye.vect() + nviewdir - v.scale(viewHeight/2) - u.scale(viewWidth/2)).toPoint();
    point lr = (eye.vect() + nviewdir - v.scale(viewHeight/2) + u.scale(viewWidth/2)).toPoint();
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
            for(int i = 0; i < spheres.size(); i++)//for each sphere (object) in scene
            {
                double t;
                if(spheres[i].intersect(curRay,t) && (closestInter > t))//returns true if intersected, assigns closer (non-neg) intersection to t
                {
                    closestInter = t;
                    closest = i;
                }
            }
            if (closest!=-1)
                imgBuf[y*imgWidth+x] = spheres[closest].shadeRay(curRay,closestInter);
        }
    }

    //CreateOutFileHeader
    fileName = fileName.replace(fileName.length()-4,fileName.length()-1,".ppm");//change extension from .txt to .ppm
    fileName = "/" + fileName; //For removing the path safely
    fileName = fileName.replace(0,fileName.find_last_of('/')+1,"");//remove path
    ofstream outFile(fileName, ofstream::trunc);
    outFile << "P3\n# Created By Dalton H.\n";
    outFile << imgWidth << " " << imgHeight << "\n";
    outFile << "255 \n";

    //Generate Output File Body
    for (int y = 0, i = 0; y < imgHeight; y++)//row major //i is for counting every 5th pixel.
    {
        for (int x = 0; x < imgWidth; x++)
        {
            outFile << (int)ceil(255*imgBuf[y*imgWidth+x].getR()) << " " << (int)ceil(255*imgBuf[y*imgWidth+x].getG()) << " " << (int)ceil(255*imgBuf[y*imgWidth+x].getB());
            if (i==4)
            {
                i=0;
                outFile << "\n";//every fifth pixel gets a new line.
            }
            else
            {
                i++;
                outFile << " ";//spacing between pixels
            }
        }
    }
    outFile.close();
    delete[] imgBuf;

    return 0;
}

//A function for my convenience. It will take an error number and output the corresponding type of error before outputting the details
int errMsg(int err, string msg="")
{
    switch (err)
    {
        case INVCALL:
            cout << "ERROR 1: Invalid Call";
            break;
        case INVFILE:
            cout << "ERROR 2: Invalid File";
            break;
        case INVKWD:
            cout << "ERROR 3: Invalid Keyword";
            break;
        case MSSKWD:
            cout << "ERROR 4: Missing Keyword";
            break;
        case MSSPRM:
            cout << "ERROR 5: Missing Parameter";
            break;
        case INVPRM:
            cout << "ERROR 6: Invalid Parameter";
            break;
        case REPKWD:
            cout << "ERROR 7: Repeated Keyword";
            break;
        default:
            cout << "ERROR ?: There was an undefined error";
            break;
    }
    cout << "\n"+msg << endl;
    return err;
}

int getInFileData(
    ifstream &inFile,
    int &width,
    int &height,
    point &eye,
    vector3 &viewdir,
    vector3 &updir,
    double &fovh,
    rgb &bkgcolor,
    vector<sphere> &spheres)//9 Parameters... =o
{
    //readline
    string inFileLine;
    getline(inFile,inFileLine);
    int lineNum = 1;

    //read data
    rgb mtlcolor;
    bool kwdIsDef [7] = {false, false, false, false, false, false, false};//we shouldn't see most keywords twice
        //we also should see mtlcolor before the first sphere
    enum kwd {EYE,VIEWDIR,UPDIR,FOVH,IMSIZE,BKGCOLOR,MTLCOLOR};//we don't need to know about sphere being defined
    while(!inFile.eof())
    {
        //determine keyword
        string keyword;
        keyword = getWord(inFileLine);

        //validate/perform keyword
        string param;
        if (keyword == "eye")
        {
            if (kwdIsDef[EYE])//if eye has already been seen
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(3,inFileLine);//might throw
                eye = point(params[0],params[1],params[2]);
                kwdIsDef[EYE] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'eye x y z\' @ Line number: " + to_string(lineNum));
            }
        }
        else if (keyword == "viewdir")
        {
            if (kwdIsDef[VIEWDIR])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(3,inFileLine);//might throw
                viewdir = vector3(params[0],params[1],params[2]);
                kwdIsDef[VIEWDIR] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'viewdir x y z\' @ Line number: " + to_string(lineNum));
            }

            if (viewdir.getX() == 0 && viewdir.getY() == 0 && viewdir.getZ() == 0) //validate value
                return errMsg(INVPRM,"viewdir is the zero vector @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "updir")
        {
            if (kwdIsDef[UPDIR])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(3,inFileLine);//might throw
                updir = vector3(params[0],params[1],params[2]);
                kwdIsDef[UPDIR] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'updir x y z\' @ Line number: " + to_string(lineNum));
            }

            if (updir.getX() == 0 && updir.getY() == 0 && updir.getZ() == 0) //validate value
                return errMsg(INVPRM,"updir is the zero vector @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "fovh")
        {
            if (kwdIsDef[FOVH])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(1,inFileLine);//might throw
                fovh = params[0];
                kwdIsDef[FOVH] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'fovh v\' @ Line number: " + to_string(lineNum));
            }

            if (fovh >=180 || fovh <= 0) //validate value
                return errMsg(INVPRM,"fovh is out of range (0,180) @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "imsize")
        {
            if (kwdIsDef[IMSIZE])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                int *params = getIntParams(2,inFileLine);//might throw
                width = params[0];
                height = params[1];
                kwdIsDef[IMSIZE] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'imsize h w\' @ Line number: " + to_string(lineNum));
            }

            if (width <= 0)
                return errMsg(INVPRM,"imsize width is out of range (0,inf) @ Line number: " + to_string(lineNum));
            if (height <= 0)
                return errMsg(INVPRM,"imsize height is out of range (0,inf) @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "bkgcolor")
        {
            if (kwdIsDef[BKGCOLOR])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(3,inFileLine);//might throw
                bkgcolor = rgb(params[0],params[1],params[2]);
                kwdIsDef[BKGCOLOR] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'bkgcolor r g b\' @ Line number: " + to_string(lineNum));
            }

            if (bkgcolor.getR() > 1.0 || bkgcolor.getR() < 0.0)
                return errMsg(INVPRM,"bkgcolor red is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (bkgcolor.getG() > 1.0 || bkgcolor.getG() < 0.0)
                return errMsg(INVPRM,"bkgcolor green is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (bkgcolor.getB() > 1.0 || bkgcolor.getB() < 0.0)
                return errMsg(INVPRM,"bkgcolor blue is out of range [0,1] @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "mtlcolor")
        {
            try
            {
                double *params = getDoubleParams(3,inFileLine);//might throw
                mtlcolor = rgb(params[0],params[1],params[2]);
                kwdIsDef[MTLCOLOR] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'mtlcolor r g b\' @ Line number: " + to_string(lineNum));
            }

            if (mtlcolor.getR() > 1.0 || mtlcolor.getR() < 0.0)
                return errMsg(INVPRM,"mtlcolor red is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (mtlcolor.getG() > 1.0 || mtlcolor.getG() < 0.0)
                return errMsg(INVPRM,"mtlcolor green is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (mtlcolor.getB() > 1.0 || mtlcolor.getB() < 0.0)
                return errMsg(INVPRM,"mtlcolor blue is out of range [0,1] @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "sphere")
        {
            if (!kwdIsDef[MTLCOLOR])//To do sphere mtlcolor must have been defined.
                return errMsg(MSSKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(4,inFileLine);//might throw
                point p(params[0],params[1],params[2]);
                spheres.push_back (sphere(p,params[3],mtlcolor));
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'sphere x y z r\' @ Line number: " + to_string(lineNum));
            }
            if (spheres[spheres.size()-1].getRadius() <= 0)
                return errMsg(INVPRM,"sphere radius is out of range (0,inf) @ Line number: " + to_string(lineNum));

        }
        else if (keyword == "")// this is actually a blank line due to the way getword and getline work.
            ; //Continue to next line
        else //unknown keyword
            return errMsg(INVKWD,"Unknown Keyword @ Line number: " + to_string(lineNum));

        //Read next line
        getline(inFile,inFileLine);
        lineNum++;
    }
    //check that the viewing params have been defined
    string kwdString[6] = {"eye","viewdir","updir","fovh","imsize","bkgcolor"};
    for (int i = 0; i <= BKGCOLOR ; i++)
        if(!kwdIsDef[i])
            return errMsg(MSSKWD,"Please define " + kwdString[i]);

    inFile.close();
    return 0;
}

/* helper function for getInFileData */
int* getIntParams(int n, string line)
{
    int * params = new int [n];
    string param;
    for (int i = 0; i < n; i++)
    {
        param = getWord(line);
        if (param == "") //end of line reached and parameters are still desired
            throw MSSPRM;
        if (!isInt(param))
            throw INVPRM;
        params[i] = atoi(param.c_str());
    }
    return params;
}

/* helper function for getInFileData */
double* getDoubleParams(int n, string line)
{
    double * params = new double [n];
    string param;
    for (int i = 0; i < n; i++)
    {
        param = getWord(line);
        if (param == "") //end of line reached and parameters are still desired
            throw MSSPRM;
        if (!isDouble(param))
            throw INVPRM;
        params[i] = atof(param.c_str());
    }
    return params;
}

//Gets the next sequence of chars in the string line. The words are delimitated by whitespace. Updates line accordingly.
string getWord(string& line) //So it turns out that C++ already has a way to do this. :/ (file/line >> word)
{
    string word = "";
    int i;
    for (i=0;isspace(line[i]) && i<(int)line.length();i++)//removes whitespace before word
        ;
    while (!isspace(line[i])&&i<(int)line.length())//reads word until whitespace
    {
        word+=line[i];
        i++;
    }
    line = &line[i];//Update line
    return word;
}

//Determines if a string is a valid integer.
bool isInt(string str)
{
    if (str.length()<1 || (str[0]!='-' && str[0]!='+' && !isdigit(str[0])))
        return false;//empty string, non-sign or non-digit first char.
    for (int i = 1; i<(int)str.length(); i++)
        if(!isdigit(str[i]))
            return false;//non digit char (after first)
    return true;
}

bool isDouble(string str)
{
    if (str.length()<1 || (str[0]!='-' && str[0]!='+' && !isdigit(str[0])))
        return false;//empty string, non-sign or non-digit first char.
    int i;
    for (i = 1; i<(int)str.length(); i++)
        if(!isdigit(str[i]))
        {
            if(str[i] == '.')
            {
                i++;
                break;//its okay if we hit one dot
            }
            else
                return false;// non-digit char
        }
    for (; i<(int)str.length(); i++)//pass over digits after dot (if there was no dot, then skip)
        if(!isdigit(str[i]))
            return false;//non-digit char

    return true;
}
