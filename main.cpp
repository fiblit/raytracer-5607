#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include "vector3.h"

using namespace std;

string getWord(string& line);
bool isInt(string str);
int getInFileData(ifstream &inFile, int &width, int &height);

int main()
{
    //GetInFileName
    string fileName;//name of input file
    ifstream inFile;//input file
    cout << "Please type the name of an existing .txt file (please include extension): ";
    getline(cin,fileName);
    inFile.open(fileName);
    if(!inFile.is_open())
    {
        cout << "ERROR 1: Nonexistant file.\nPlease enter the name of an existing .txt file.";
        return 1;
    }
    if(fileName.length()<=4 || fileName.substr(fileName.length()-4,fileName.length()-1)!=".txt")//the length <=4 is to make sure substr can be run in bounds.
    {
        cout << "ERROR 2: Incorrect file extension.\nPlease enter a .txt file.";
        inFile.close();
        return 2;
    }

    int errval;
    int imgWidth, imgHeight;
    if ((errval = getInFileData(inFile, imgWidth, imgHeight)))
        return errval;

    //CreateOutFileHeader
    fileName = fileName.replace(fileName.length()-4,fileName.length()-1,".ppm");//change extension from .txt to .ppm
    ofstream outFile(fileName, ofstream::trunc);
    outFile << "P3\n# Created By Dalton H.\n";
    outFile << imgWidth << " " << imgHeight << "\n";
    outFile << "255 \n";

    //GenerateInterestingImage
    int x,y,i=0;//i is for counting every 5th pixel.
    for (y = 0; y < imgHeight; y++)//row major
    {
        for(x = 0; x < imgWidth; x++)
        {
            outFile << (y*255)/(imgHeight-1) << " 128 " << (x*255)/(imgWidth-1);//The -1 is for getting a range of [0,255] rather than [0,254].
            //Green is 128 because I couldn't think of an easier way to make it look nice. :P
            if(i==4)
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

    return 0;
}

//eye (x,y,z) //point
//viewdir (x,y,z) //vector
//updir (x,y,z) //vector
//fovh //int?
//bkgcolor (r,g,b) //rgb
//mtlcolors (r,g,b) //rgb
//spheres (x,y,z,r) //sphere (object/sphere?)
int getInFileData(ifstream &inFile, int &width, int &height)
{
    string inFileLine;
    //TODO: while !@EOF switch statement over header
    getline(inFile,inFileLine);//get keyword
    if(getWord(inFileLine)!="imsize")
    {
        cout << "ERROR 3: Invalid header keyword.\nPlease place on the first line \"imsize <width> <height>\"." << endl;
        inFile.close();
        return 3;
    }
    string strWidth,strHeight;

    strWidth=getWord(inFileLine);//get width
    if(strWidth.length()<1)
    {
        cout << "ERROR 4: Nonexistant width parameter.\nPlease place on the first line \"imsize <width> <height>\"." << endl;
        inFile.close();
        return 4;
    }
    strHeight=getWord(inFileLine);//get heighht
    if(strHeight.length()<1)
    {
        cout << "ERROR 4: Nonexistant height parameter.\nPlease place on the first line \"imsize <width> <height>\"." << endl;
        inFile.close();
        return 4;
    }
    if(!isInt(strWidth) || !isInt(strHeight))
    {//Checks for non integer type. First two statements check for non-numeral, second two check for floating point.
        cout << "ERROR 5: Invalid width or height parameter.\nPlease have integer width and height parameters." << endl;
        inFile.close();
        return 5;
    }
    width = atoi(strWidth.c_str());
    height = atoi(strHeight.c_str());
    if (width<1 || height < 1)
    {
        cout << "ERROR 5: Invalid width or height parameter.\nPlease have positive width and height parameters." << endl;
        inFile.close();
        return 5;
    }
    inFile.close();
    return 0;
}

//Gets the next sequence of chars in the string line. The words are delimitated by whitespace. Updates line accordingly.
string getWord(string& line)
{
    string word = "";
    int i;
    for(i=0;isspace(line[i]) && i<(int)line.length();i++)//removes whitespace before word
        ;
    while(!isspace(line[i])&&i<(int)line.length())//reads word until whitespace
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
    if(str.length()<1||(str[0]!='-'&&str[0]!='+'&&!isdigit(str[0])))
        return false;//empty string, non-sign or digit first char.
    for(int i = 1; i<(int)str.length();i++)
        if(!isdigit(str[i]))
            return false;//non digit char (after first)
    return true;
}
