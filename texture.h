#ifndef TEXTURE_H
#define TEXTURE_H

#include "rgb.h"

using namespace std;

class texture
{
    public:
        texture();
        texture(rgb **img, int height, int width);
        rgb **getImg();
        int getHeight();
        int getWidth();
        void setImg(rgb **img);
        void setHeight(int height);
        void setWidth(int width);
    private:
        rgb **img;
        int height;
        int width;
};

#endif // TEXTURE_H
