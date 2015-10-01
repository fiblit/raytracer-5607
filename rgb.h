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
        double getR() const;
        double getG() const;
        double getB() const;
        rgb operator* (const double &s);
        rgb operator* (const rgb &c);
        rgb operator+ (const rgb &c);
    private:
        double r;
        double g;
        double b;
};

#endif // RGB_H
