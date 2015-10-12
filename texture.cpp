#include "texture.h"

texture::texture() { }

texture::texture(rgb **img, int height, int width)
{
    this->img = img;
    this->height = height;
    this->width = width;
}

rgb **texture::getImg()
{
    return img;
}

int texture::getHeight()
{
    return height;
}

int texture::getWidth()
{
    return width;
}

void texture::setImg(rgb **img)
{
    this->img = img;
}

void texture::setHeight(int height)
{
    this->height = height;
}

void texture::setWidth(int width)
{
    this->width = width;
}
