#include"math.h"

#include<algorithm>
#include<cmath>

double Utils::uniform()
{
    const int rand_limit=1024;
    return 1.*(rand()%rand_limit)/(rand_limit-1);
}
double Utils::deg2rad(double deg)
{
    return deg*pi/180.;
}

template<typename T>
double Vector<T>::normal() const
{
    return std::sqrt(x*x+y*y+z*z);
}
template<typename T>
void Vector<T>::normalize()
{
    double n=normal();
    x/=n,y/=n,z/=n;
}
template<typename T>
Vector<T> Vector<T>::normalized() const
{
    double n=normal();
    return Vector(x/n,y/n,z/n);
}
template<typename T>
T Vector<T>::dot_product(const Vector<T> &rhx) const
{
    return x*rhx.x+y*rhx.y+z*rhx.z;
}
template<typename T>
Vector<T> Vector<T>::cross_product(const Vector<T> &rhx) const
{
    return Vector(y*rhx.z-rhx.y*z,rhx.x*z-x*rhx.z,x*rhx.y-rhx.x*y);
}
template<typename T>
Vector<T> Vector<T>::hadamard_product(const Vector<T> &rhx) const
{
    return Vector(x*rhx.x,y*rhx.y,z*rhx.z);
}
template<typename T>
Vector<T> Vector<T>::operator*(T s) const
{
    return Vector(x*s,y*s,z*s);
}
template<typename T>
Vector<T> Vector<T>::operator+(T k) const
{
    return Vector(x+k,y+k,z+k);
}
template<typename T>
T Vector<T>::operator[](unsigned int k) const
{
    switch(k)
    {
        case 1: return x;
        case 2: return y;
        case 3: return z;
        default: return 0.;
    }
}
template<typename T>
Vector<T> Vector<T>::operator+(const Vector<T> &rhx) const
{
    return Vector(x+rhx.x,y+rhx.y,z+rhx.z);
}
template<typename T>
Vector<T> Vector<T>::operator-(const Vector<T> &rhx) const
{
    return Vector(x-rhx.x,y-rhx.y,z-rhx.z);
}