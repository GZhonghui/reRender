#ifndef MATH_H
#define MATH_H

#include<cmath>

const double eps=1e-6;
const double pi=acos(-1.);

class Utils
{
public:
    Utils()=default;
    ~Utils()=default;
public:
    static double uniform();
    static double deg2rad(double deg);
};

template<typename T>
class Vector
{
protected:
    T x,y,z;
public:
    Vector()
    {
        x=y=z=(T)0;
    }
    Vector(T k)
    {
        x=y=z=k;
    }
    Vector(T x,T y,T z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    ~Vector()=default;
public:
    double normal() const;
    void normalize();
    Vector normalized() const;
    T dot_product(const Vector &rhx) const;
    Vector cross_product(const Vector &rhx) const;
    Vector hadamard_product(const Vector &rhx) const;
    Vector operator*(T s) const;
    Vector operator+(T k) const;
    T operator[](unsigned int k) const;
    Vector operator+(const Vector &rhx) const;
    Vector operator-(const Vector &rhx) const;
};

template class Vector<int>;
template class Vector<double>;

typedef Vector<int> Vectori;
typedef Vector<int> Box;
typedef Vector<int> Triface;
typedef Vector<double> Vectorf;
typedef Vector<double> Color;
typedef Vector<double> Point;
typedef Vector<double> Direction;

#endif