#include "IO.h"
#include "object.h"
#include "sphere.h"
#include "cylinder.h"
#include "triangle.h"

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
    bool kwdIsDef [9] = {false, false, false, false, false, false, false, false, false};//we shouldn't see most keywords twice
        //we also should see mtlcolor before the first sphere
    enum kwd {EYE, VIEWDIR, UPDIR, FOVH, IMSIZE, BKGCOLOR, MTLCOLOR, PARALLEL, TEXTURE};//we don't need to know about sphere (objects) being defined
    while(!inFile.eof())
    {
        //determine keyword
        string keyword;
        keyword = getWord(inFileLine);

        //validate/perform keyword
        string param;
        if (keyword == "eye")//TODO: add handler functions for each keyword.
        {
            if (kwdIsDef[EYE])//if eye has already been seen
                return errMsg(REPKWD,"Line number: " + to_string(lineNum));
            try
            {
                double *params = getDoubleParams(3,inFileLine);//might throw
                *fd.eye = point(params[0],params[1],params[2]);
                kwdIsDef[EYE] = true;

                delete[] params;
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

                delete[] params;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'viewdir x y z\' @ Line number: " + to_string(lineNum));
            }

            if (abs((*fd.viewdir).getX()) < EPSILON &&
                abs((*fd.viewdir).getY()) < EPSILON &&
                abs((*fd.viewdir).getZ()) < EPSILON) //validate value
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

                delete[] params;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'updir x y z\' @ Line number: " + to_string(lineNum));
            }

            if (abs((*fd.updir).getX()) < EPSILON &&
                abs((*fd.updir).getY()) < EPSILON &&
                abs((*fd.updir).getZ()) < EPSILON) //validate value
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

                delete[] params;
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

                delete[] params;
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

                delete[] params;
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

                delete[] params;
                delete[] paramsI;
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
                int texIndex = -1;//-1 means no texture is defined (NULL)
                if (kwdIsDef[TEXTURE])
                    texIndex = (*fd.textures).size() - 1;

                (*fd.objects).push_back (new sphere(p, params[3], mtlcolor, texIndex));

                if (params[3] <=0)
                    return errMsg(INVPRM,"sphere radius is out of range (0,inf) @ Line number: " + to_string(lineNum));

                delete[] params;
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
                int *paramsPnt = getIntParams(1, inFileLine);
                double *paramsRGB = getDoubleParams(3, inFileLine);
                vector3 loc(paramsLoc[0], paramsLoc[1], paramsLoc[2]);
                rgb color(paramsRGB[0], paramsRGB[1], paramsRGB[2]);
                (*fd.lights).push_back (light(loc, (bool)paramsPnt[0], color));

                delete[] paramsLoc;
                delete[] paramsPnt;
                delete[] paramsRGB;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'light x y z w r g b\' @ Line number: " + to_string(lineNum));
            }

            light l = (*fd.lights)[(*fd.lights).size() - 1];//for convenience
            if (!l.getIsPnt() &&
                abs(l.getLoc().getX()) < EPSILON &&
                abs(l.getLoc().getY()) < EPSILON &&
                abs(l.getLoc().getZ()) < EPSILON)
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

                delete[] params;
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
        else if (keyword == "texture")
        {
            try
            {
                string *tex = getStringParams(1, inFileLine);
                if (tex[0].length()<=4 || tex[0].substr(tex[0].length()-4, tex[0].length()-1)!=".ppm")
                    return errMsg(INVFILE, "Please use a .ppm file @ Line Number: " + to_string(lineNum));

                ifstream texFile(tex[0]);
                if (!texFile.is_open())
                    return errMsg(INVFILE, "Please use an existing .ppm file @ Line Number: " + to_string(lineNum));

                try
                {
                    (*fd.textures).push_back (getTexture(texFile));
                }
                catch(errNum e)
                {
                    return errMsg(e, "There was an error in reading the texture @ Line Number: " + to_string(lineNum));
                }

                kwdIsDef[TEXTURE] = true;
                texFile.close();
                delete[] tex;
            }
            catch(errNum e)
            {
                return errMsg(e, "Usage \'texture texture_file.ppm\' @ Line Number: " + to_string(lineNum));
            }
        }
        else if (keyword == "v")
        {
            try
            {
                double *loc = getDoubleParams(3, inFileLine);//might throw
                (*fd.vertices).push_back (point(loc[0], loc[1], loc[2]) );

                delete[] loc;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'v x y z\' @ Line number: " + to_string(lineNum));
            }
        }
        else if (keyword == "vt")
        {
            try
            {
                double *coords = getDoubleParams(2, inFileLine);//might throw
                textureCoord uv;
                uv.u = coords[0];
                uv.v = coords[1];
                (*fd.vTextures).push_back (uv);

                delete[] coords;
            }
            catch(errNum e)
            {
                return errMsg(e, "Usage \'vt u v\' @ Line number: " + to_string(lineNum));
            }
        }
        else if (keyword == "vn")
        {
            try
            {
                double *norm = getDoubleParams(3, inFileLine);//might throw
                (*fd.vNormals).push_back (vector3(norm[0], norm[1], norm[2]) );

                delete[] norm;
            }
            catch(errNum e)
            {
                return errMsg(e,"Usage \'vn dx dy dz\' @ Line number: " + to_string(lineNum));
            }

            if (abs( (*fd.vNormals)[(*fd.vNormals).size() - 1].getX() ) < EPSILON &&
                abs( (*fd.vNormals)[(*fd.vNormals).size() - 1].getY() ) < EPSILON &&
                abs( (*fd.vNormals)[(*fd.vNormals).size() - 1].getZ() ) < EPSILON) //validate value
                return errMsg(INVPRM,"vn is the zero vector @ Line number: " + to_string(lineNum));
        }
        else if (keyword == "f") // this one got kinda big because getDoubleParams and getIntParams couldn't be used anymore.
        {
            if (!kwdIsDef[MTLCOLOR])
                return errMsg(MSSKWD,"Need mtlcolor before this line @ Line number: " + to_string(lineNum));

            int vParams[3] = {-1, -1, -1};
            int vtParams[3] = {-1, -1, -1};
            int vnParams[3] = {-1, -1, -1};
            bool hasVt;
            int formatSplitSize;

            for (int i = 0; i < 3; i++) //handle the varying syntax of 'f' (which screwed with my modularity)
            {
                string param = getWord(inFileLine);
                vector<string> vertexParam = split(param, "/");

                if (i == 0)
                    formatSplitSize = vertexParam.size();
                else if ((int)vertexParam.size() != formatSplitSize)
                    return errMsg(INVPRM, "Please use consistent format for the face @ line number: " + to_string(lineNum));

                if (vertexParam.size() == 1)//v1 v2 v3 format
                {
                    if (vertexParam[0] == "")
                        return errMsg(MSSPRM, "Missing a vertex @ Line number: " + to_string(lineNum));

                    if (!isInt(vertexParam[0]))
                        return errMsg(INVPRM, "Invalid type (use integers) @ Line number: " + to_string(lineNum));

                    vParams[i] = atoi(vertexParam[0].c_str()) - 1;//-1 because of obj format

                    if (vParams[i] < 0 || vParams[i] > (int)(*fd.vertices).size() - 1)
                        return errMsg(INVPRM, "vertexIndex out of range 1-#v's @ Line number: " + to_string(lineNum));
                }
                else if (vertexParam.size() == 2)//v/vt format
                {
                    if (vertexParam[0] == "")
                        return errMsg(MSSPRM, "Missing a vertex @ Line number: " + to_string(lineNum));
                    if (vertexParam[1] == "")
                        return errMsg(MSSPRM, "Missing a vertex texture coord @ Line number: " + to_string(lineNum));

                    if (!isInt(vertexParam[0]))
                        return errMsg(INVPRM, "Invalid type (use integers) @ Line number: " + to_string(lineNum));
                    if (!isInt(vertexParam[1]))
                        return errMsg(INVPRM, "Invalid type (use integers) @ Line number: " + to_string(lineNum));

                    vParams[i] = atoi(vertexParam[0].c_str()) - 1;
                    vtParams[i] = atoi(vertexParam[1].c_str()) - 1;

                    if (vParams[i] < 0 || vParams[i] > (int)(*fd.vertices).size() - 1)
                        return errMsg(INVPRM, "vertexIndex out of range 1-#v's @ Line number: " + to_string(lineNum));
                    if (vtParams[i] < 0 || vtParams[i] > (int)(*fd.vTextures).size() - 1)
                        return errMsg(INVPRM, "vTextureIndex out of range 1-#vt's @ Line number: " + to_string(lineNum));
                }
                else if (vertexParam.size() == 3)// v/(vt)/vn format
                {
                    if (vertexParam[0] == "")
                        return errMsg(MSSPRM, "Missing a vertex @ Line number: " + to_string(lineNum));
                    if (vertexParam[2] == "")
                        return errMsg(MSSPRM, "Missing a vertex normal @ Line number: " + to_string(lineNum));
                    if (i == 0)
                        hasVt = (vertexParam[1] != "");
                    else
                    {
                        if ((hasVt && (vertexParam[1] == "")) || (!hasVt && (vertexParam[1] != "")))//inconsistent vt
                            return errMsg(MSSPRM, "There needs to be a vt defined for all vertices, since one is defined. @ Line number: " + to_string(lineNum));
                    }

                    if (!isInt(vertexParam[0]))
                        return errMsg(INVPRM, "Invalid type (use integers) @ Line number: " + to_string(lineNum));
                    if (hasVt && !isInt(vertexParam[1]))//need to make sure we're doing vt
                        return errMsg(INVPRM, "Invalid type (use integers) @ Line number: " + to_string(lineNum));
                    if (!isInt(vertexParam[2]))
                        return errMsg(INVPRM, "Invalid type (use integers) @ Line number: " + to_string(lineNum));

                    vParams[i] = atoi(vertexParam[0].c_str()) - 1;
                    if (hasVt)
                        vtParams[i] = atoi(vertexParam[1].c_str()) - 1;
                    vnParams[i] = atoi(vertexParam[2].c_str()) - 1;

                    if (vParams[i] < 0 || vParams[i] > (int)(*fd.vertices).size() - 1)
                        return errMsg(INVPRM, "vertexIndex out of range 1-#v's @ Line number: " + to_string(lineNum));
                    if (hasVt && (vtParams[i] < 0 || vtParams[i] > (int)(*fd.vTextures).size() - 1))
                        return errMsg(INVPRM, "vertexIndex out of range 1-#vt's @ Line number: " + to_string(lineNum));
                    if (vnParams[i] < 0 || vnParams[i] > (int)(*fd.vNormals).size() - 1)
                        return errMsg(INVPRM, "vertexIndex out of range 1-#vn's @ Line number: " + to_string(lineNum));
                }
                else
                    return errMsg(INVPRM, "Usage \'f v1 v2 v3\' or \'f v1//vn1 v2//vn2 v3//vn3\' or \'f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3\' @ Line number: " + to_string(lineNum));
            }

            int texIndex = -1;//-1 means no texture is defined (NULL)
            if (kwdIsDef[TEXTURE])
                texIndex = (*fd.textures).size() - 1;

            (*fd.objects).push_back(new triangle(vParams, vtParams, vnParams, mtlcolor, texIndex));

            //No deletes since the params aren't on the heap this time :P
        }
        else if (keyword[0] == '#')//I AM A COMMENT
            ;//                     ^^^ He is a comment
        else if (keyword == "")// this is actually a blank line due to the way getword and getline work.
            ; //Continue to next line
        else
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

/* reads a line for n ints */
int *getIntParams(int n, string &line)
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

/* reads a line for n doubles */
double *getDoubleParams(int n, string &line)
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

/* reads a line for n strings (words) */
string *getStringParams(int n, string &line)
{
    string *params = new string[n];
    string param;
    for (int i = 0; i < n; i++)
    {
        param = getWord(line);
        if (param == "")//end of line reached and parameters are still desired
            throw MSSPRM;
        params[i] = param;
    }
    return params;
}

texture getTexture(ifstream &texFile)
{
    string line;
    getline(texFile, line);
    if (getWord(line)!="P3")
        throw INVFILE;
    int *header = getIntParams(3, line);
    int width = header[0];
    int height = header[1];
    int value = header[2];
    delete[] header;

    rgb **textureImg = new rgb*[height];
    for (int i = 0; i < height; i++)
        textureImg[i] = new rgb[width];

    string r;//Stupid GIMP
    string g;
    string b;
    getline(texFile, r);
    getline(texFile, g);
    getline(texFile, b);
    for(int lineNum = 0; !texFile.eof(); lineNum++)
    {
        int *pixelr = getIntParams(1, r);
        int *pixelg = getIntParams(1, g);
        int *pixelb = getIntParams(1, b);
        textureImg[lineNum/width][lineNum%width] = rgb(((double)pixelr[0])/value, ((double)pixelg[0])/value, ((double)pixelb[0])/value);
        getline(texFile, r);
        getline(texFile, g);
        getline(texFile, b);

        delete[] pixelr;
        delete[] pixelg;
        delete[] pixelb;
    }

    return texture(textureImg, width, height);
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

vector<string> split(string &str, const string delims)
{
    vector<string> result;
    bool endNull = false;
    while(!str.empty())
    {
        string word = "";
        int i = 0;
        while (delims.find(str[i])==string::npos && i<(int)str.length())//while no delim
        {
            word+=str[i];
            i++;
        }
        if(delims.find(str[i])!=string::npos && i==(int)str.length()-1)//this is an edge case where something like "1,2," should be ("1","2","") but is ("1","2")
            endNull = true;
        if (delims.find(str[i])!=string::npos)
            i++;
        str = &str[i];//Update line
        result.push_back(word);
    }
    if (endNull)
        result.push_back("");
    return result;
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
