#include "IO.h"

using namespace std;

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
        default://This should never happen. (Unless I'm lazy)
            cout << "ERROR ?: There was an undefined error";
            break;
    }
    cout << "\n"+msg << endl;
    return err;
}

/*
Reads all of the data from inFile into the other parameters.
Also, does some validation on the data. (e.g. type checking, range checking)
*/
int getInFileData(ifstream &inFile, fileData_t fd)
{
    //readline
    string inFileLine;
    getline(inFile,inFileLine);
    int lineNum = 1;

    //read data
    material mtlcolor;
    bool kwdIsDef [8] = {false, false, false, false, false, false, false, false};//we shouldn't see most keywords twice
        //we also should see mtlcolor before the first sphere
    enum kwd {EYE,VIEWDIR,UPDIR,FOVH,IMSIZE,BKGCOLOR,MTLCOLOR,PARALLEL};//we don't need to know about sphere being defined
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
                *fd.eye = point(params[0],params[1],params[2]);
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
                *fd.viewdir = vector3(params[0],params[1],params[2]);
                kwdIsDef[VIEWDIR] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'viewdir x y z\' @ Line number: " + to_string(lineNum));
            }

            if ((*fd.viewdir).getX() == 0 && (*fd.viewdir).getY() == 0 && (*fd.viewdir).getZ() == 0) //validate value
                return errMsg(INVPRM,"viewdir is the zero vector @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "updir")
        {
            if (kwdIsDef[UPDIR])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(3,inFileLine);//might throw
                *fd.updir = vector3(params[0],params[1],params[2]);
                kwdIsDef[UPDIR] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'updir x y z\' @ Line number: " + to_string(lineNum));
            }

            if ((*fd.updir).getX() == 0 && (*fd.updir).getY() == 0 && (*fd.updir).getZ() == 0) //validate value
                return errMsg(INVPRM,"updir is the zero vector @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "fovh")
        {
            if (kwdIsDef[FOVH])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(1,inFileLine);//might throw
                *fd.fovh = params[0];
                kwdIsDef[FOVH] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'fovh v\' @ Line number: " + to_string(lineNum));
            }

            if (*fd.fovh >=180 || *fd.fovh <= 0) //validate value
                return errMsg(INVPRM,"fovh is out of range (0,180) @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "imsize")
        {
            if (kwdIsDef[IMSIZE])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                int *params = getIntParams(2,inFileLine);//might throw
                *fd.width = params[0];
                *fd.height = params[1];
                kwdIsDef[IMSIZE] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'imsize h w\' @ Line number: " + to_string(lineNum));
            }

            if (*fd.width <= 0)
                return errMsg(INVPRM,"imsize width is out of range (0,inf) @ Line number: " + to_string(lineNum));
            if (*fd.height <= 0)
                return errMsg(INVPRM,"imsize height is out of range (0,inf) @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "bkgcolor")
        {
            if (kwdIsDef[BKGCOLOR])
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(3,inFileLine);//might throw
                *fd.bkgcolor = rgb(params[0],params[1],params[2]);
                kwdIsDef[BKGCOLOR] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'bkgcolor r g b\' @ Line number: " + to_string(lineNum));
            }

            if ((*fd.bkgcolor).getR() > 1.0 || (*fd.bkgcolor).getR() < 0.0)
                return errMsg(INVPRM,"bkgcolor red is out of range [0,1] @ Line number: " + to_string(lineNum));
            if ((*fd.bkgcolor).getG() > 1.0 || (*fd.bkgcolor).getG() < 0.0)
                return errMsg(INVPRM,"bkgcolor green is out of range [0,1] @ Line number: " + to_string(lineNum));
            if ((*fd.bkgcolor).getB() > 1.0 || (*fd.bkgcolor).getB() < 0.0)
                return errMsg(INVPRM,"bkgcolor blue is out of range [0,1] @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "mtlcolor")
        {
            try
            {
                double *params = getDoubleParams(9, inFileLine);//might throw
                int *paramsI = getIntParams(1, inFileLine);//Since n should be int, not double.
                mtlcolor = material(rgb(params[0],params[1],params[2]), rgb(params[3],params[4],params[5]), params[6], params[7], params[8], paramsI[0]);
                kwdIsDef[MTLCOLOR] = true;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'mtlcolor Dr Dg Db Sr Sg Sb ka kd ks n\' @ Line number: " + to_string(lineNum));
            }

            if (mtlcolor.getOd().getR() > 1.0 || mtlcolor.getOd().getR() < 0.0)
                return errMsg(INVPRM,"mtlcolor diffuse red is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (mtlcolor.getOd().getG() > 1.0 || mtlcolor.getOd().getG() < 0.0)
                return errMsg(INVPRM,"mtlcolor diffuse green is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (mtlcolor.getOd().getB() > 1.0 || mtlcolor.getOd().getB() < 0.0)
                return errMsg(INVPRM,"mtlcolor diffuse blue is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (mtlcolor.getOs().getR() > 1.0 || mtlcolor.getOs().getR() < 0.0)
                return errMsg(INVPRM,"mtlcolor specular red is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (mtlcolor.getOs().getG() > 1.0 || mtlcolor.getOs().getG() < 0.0)
                return errMsg(INVPRM,"mtlcolor specular green is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (mtlcolor.getOs().getB() > 1.0 || mtlcolor.getOs().getB() < 0.0)
                return errMsg(INVPRM,"mtlcolor specular blue is out of range [0,1] @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "sphere")
        {
            if (!kwdIsDef[MTLCOLOR])//To do sphere mtlcolor must have been defined.
                return errMsg(MSSKWD,"Need mtlcolor before this line @ Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(4,inFileLine);//might throw
                point p(params[0],params[1],params[2]);
                (*fd.objects).push_back (new sphere(p,params[3],mtlcolor));

                if (params[3] <=0)
                    return errMsg(INVPRM,"sphere radius is out of range (0,inf) @ Line number: " + to_string(lineNum));
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'sphere x y z r\' @ Line number: " + to_string(lineNum));
            }

            //if ((*fd.objects)[(*fd.objects).size()-1].getRadius() <= 0)
            //    return errMsg(INVPRM,"sphere radius is out of range (0,inf) @ Line number: " + to_string(lineNum));

        }
        else if (keyword == "light")
        {
            try
            {
                double *paramsLoc = getDoubleParams(3, inFileLine);//the getParams might throw
                int *paramsDir = getIntParams(1, inFileLine);
                double *paramsRGB = getDoubleParams(3, inFileLine);
                vector3 loc(paramsLoc[0], paramsLoc[1], paramsLoc[2]);
                rgb color(paramsRGB[0], paramsRGB[1], paramsRGB[2]);
                (*fd.lights).push_back (light(loc, !(bool)paramsDir[0], color));//The not is to deal with the 1/0 confusion. I'll have to change it.
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'light x y z w r g b\' @ Line number: " + to_string(lineNum));
            }

            light l = (*fd.lights)[(*fd.lights).size() - 1];//for convenience
            if (l.getIsDir() && l.getLoc().getX() == 0 && l.getLoc().getY() == 0 && l.getLoc().getZ() == 0 )
                return errMsg(INVPRM,"light dir is the zero vector @ Line number: " + to_string(lineNum));
            if (l.getColor().getR() > 1.0 || l.getColor().getR() < 0.0)
                return errMsg(INVPRM,"light red is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (l.getColor().getG() > 1.0 || l.getColor().getG() < 0.0)
                return errMsg(INVPRM,"light green is out of range [0,1] @ Line number: " + to_string(lineNum));
            if (l.getColor().getB() > 1.0 || l.getColor().getB() < 0.0)
                return errMsg(INVPRM,"light blue is out of range [0,1] @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "cylx" || keyword == "cyly" || keyword == "cylz")
        {
            if (!kwdIsDef[MTLCOLOR])//To do cylinder mtlcolor must have been defined.
                return errMsg(MSSKWD,"Need mtlcolor before this line @ Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(5, inFileLine);//might throw
                cylinder::cylTypes type;
                if (keyword == "cylx")
                    type = cylinder::cylTypes::X;
                else if (keyword == "cyly")
                    type = cylinder::cylTypes::Y;
                else
                    type = cylinder::cylTypes::Z;
                (*fd.objects).push_back (new cylinder(params[0], params[1], params[2], params[3], params[4], type, mtlcolor));

                if (params[3] > params[4])//minw > maxw
                    return errMsg(INVPRM, "cylinder minw is greater than maxw @ Line number: " + to_string(lineNum));
                if (params[2] <= 0)
                    return errMsg(INVPRM, "cylinder radius is out of range (0,inf) @ Line number: " + to_string(lineNum));
            }
            catch(errNum e)
            {
                return errMsg(e, "Usage \'cylx/y/z x/y y/z r\' @ Line number: " + to_string(lineNum));
            }
        }
        else if (keyword == "parallel")
        {
            if (kwdIsDef[PARALLEL])
                return errMsg(REPKWD, "please use on parallel keyword. Remove parallel @ Line number: " + to_string(lineNum));
            *fd.parallel = true;
            kwdIsDef[PARALLEL] = true;
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
    if (!kwdIsDef[PARALLEL])
        *fd.parallel = false;

    inFile.close();
    return 0;
}

// helper function for getInFileData
int* getIntParams(int n, string &line)
{
    int *params = new int [n];
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

// helper function for getInFileData
double* getDoubleParams(int n, string &line)
{
    double *params = new double [n];
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

void writeOutFile(string outFileName, rgb **imgBuf, int imgWidth, int imgHeight)
{
    //Create Output File & Header
    ofstream outFile(outFileName, ofstream::trunc);
    if(!outFile.is_open())
        throw errMsg(INVFILE, "The output file could not open. The path might not exist.");
    outFile << "P3\n# Created By Dalton Hildreth\n";
    outFile << imgWidth << " " << imgHeight << "\n";
    outFile << "255 \n";

    //Generate Output File Body
    for (int y = 0, i = 0; y < imgHeight; y++)//row major //i is for counting every 5th pixel.
    {
        for (int x = 0; x < imgWidth; x++)
        {
            outFile << (int)ceil(255*imgBuf[y][x].getR()) << " " << (int)ceil(255*imgBuf[y][x].getG()) << " " << (int)ceil(255*imgBuf[y][x].getB());
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
}

//Gets the next sequence of chars in the string line. The words are delimitated by whitespace. Updates line accordingly.
string getWord(string &line) //So it turns out that C++ already has a way to do this. :/ (file/line >> word)
{
    string word = "";
    int i;
    for (i=0;isspace(line[i]) && i<(int)line.length();i++)//removes whitespace before word
        ;
    while (!isspace(line[i]) && i<(int)line.length())//reads word until whitespace
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

//Determines if a string is a valid double
bool isDouble(string str)
{
    if (str.length()<1 || (str[0]!='-' && str[0]!='+' && !isdigit(str[0]) && str[0]!='.'))
        return false;//empty string, non-sign or non-digit first char.

    bool dotSeen = false;
    if(str[0]=='.')
        dotSeen = true;

    int i;
    for (i = 1; i<(int)str.length(); i++)
        if(!isdigit(str[i]))
        {
            if(str[i] == '.' && !dotSeen)
            {
                i++;
                break;//its okay if we hit one dot
            }
            else
                return false;// non-digit char or dot already seen
        }
    for (; i<(int)str.length(); i++)//pass over digits after dot (if there was no dot, then skip)
        if(!isdigit(str[i]))
            return false;//non-digit char

    return true;
}
