#ifndef RAY_H
#define RAY_H


class ray
{
    public:
        ray();
        ray(point origin, vector3 dir);
        ray(double x0, double y0, double z0, double xd, double yd, double zd);

    protected:
    private:
        point origin;
        vector3 dir;
};

#endif // RAY_H
