#ifndef RGB_H
#define RGB_H


class rgb
{
    public:
        rgb();
        rgb(double r, double g, double b);
        void setRGB(double r, double g, double b);
        void setR(double r);
        void setG(double g);
        void setB(double b);
        double getR();
        double getG();
        double getB();
    private:
        double r;
        double g;
        double b;
};

#endif // RGB_H
