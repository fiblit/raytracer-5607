#ifndef TEXTURECOORD_H
#define TEXTURECOORD_H

//This caused so many more problems than benefits. I should have just done an int[2] for the vt vector list. Dx
//I take that back. Having just an int[2] would have been awkward to read.
//I take THAT back, it wouldn't really be awkward to see vTexture[n][0] to signify the nth u. :P I'm removing this soon. (after the next commit)
typedef struct vt
{
    double u;
    double v;
} textureCoord;

#endif // TEXTURECOORD_H
