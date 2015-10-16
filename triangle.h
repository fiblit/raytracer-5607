#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"
#include "vector3.h"
#include "object.h"
#include "material.h"

typedef struct vt {
    int u;
    int v;
} vertexTexture;

class triangle: public object {//equivalent to an 'f' in the input.
    public:
        triangle();
        triangle(int v[3]);
        triangle(int v[3], int vn[3]);
        triangle(int v[3], int vt[3], int vn[3]);
        bool intersect(ray rr, double &t);
        rgb shadeRay(ray rr, double t, vector<light> lights, vector<object*> objects, vector<texture> textures);
        int getv(int i);
        int getvt(int i);
        int getvn(int i);
        material getMtl();
        int getTexIndex();
        void setv(int v, int i);
        void setvt(int vt, int i);
        void setvn(int vn, int i);
        void setMtl(material mtl);
        void setTexIndex(int i);

    private:
        int v[3];//vertices
        int vt[3];//vertex texture coords
        int vn[3];//vertex normals
        material mtl; // TODO: change to mtlIndex
        int texIndex;

        int beta;//I couldn't think of a better way not to recompute beta/gamma
        int gamma;
};

#endif // TRIANGLE_H
