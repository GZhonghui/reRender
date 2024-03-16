#ifndef VECTOR_H
#define VECTOR_H

#include<algorithm>
#include<cstdlib>
#include<cmath>

#include"basic_math.h"

#ifndef DOUBLE_FLOAT

//Use float instead of double

#endif

class Vector3f
{
public:
    double x,y,z;
public:
    Vector3f():x(0),y(0),z(0){}
    Vector3f(double k):x(k),y(k),z(k){}
    Vector3f(double x,double y,double z):x(x),y(y),z(z){}
    ~Vector3f()=default;
public:
    Vector3f operator+(const Vector3f &rhx) const
    {
        return Vector3f(x+rhx.x,y+rhx.y,z+rhx.z);
    }
    Vector3f operator-(const Vector3f &rhx) const
    {
        return Vector3f(x-rhx.x,y-rhx.y,z-rhx.z);
    }
    Vector3f operator*(double k) const
    {
        return Vector3f(x*k,y*k,z*k);
    }
    Vector3f operator/(double k) const
    {
        return Vector3f(x/k,y/k,z/k);
    }
    double operator[](unsigned int k) const
    {
        switch(k)
        {
            case 1:return x;
            case 2:return y;
            case 3:return z;
        }
        return 0;
    }
    bool operator<(const Vector3f &rhx) const
    {
        for(unsigned int i=1;i<=3;i+=1)
        {
            if(operator[](i)>rhx[i])
            {
                return false;
            }
        }
        return true;
    }
public:
    Vector3f hadamard_product(const Vector3f &rhx) const
    {
        return Vector3f(x*rhx.x,y*rhx.y,z*rhx.z);
    }
    Vector3f cross_product(const Vector3f &rhx) const
    {
        return Vector3f(y*rhx.z-rhx.y*z,rhx.x*z-x*rhx.z,x*rhx.y-rhx.x*y);
    }
    double dot_product(const Vector3f &rhx) const
    {
        return x*rhx.x+y*rhx.y+z*rhx.z;
    }
public:
    Vector3f opposite() const
    {
        return Vector3f(-x,-y,-z);
    }
    double normal() const
    {
        return std::sqrt(x*x+y*y+z*z);
    }
    Vector3f normalized() const
    {
        return operator/(normal());
    }
    double distance(const Vector3f &rhx) const
    {
        return operator-(rhx).normal();
    }
    Vector3f middle(const Vector3f &rhx) const
    {
        return operator+(rhx).normalized();
    }
public:
    void normalize()
    {
        (*this)=normalized();
    }
    void limit(double l,double r)
    {
        if(l>r) return;
        
        using std::min;
        using std::max;
        x=max(l,min(r,x));
        y=max(l,min(r,y));
        z=max(l,min(r,z));
    }
public:
    static Vector3f random()
    {
        return Vector3f(Utils::uniform(),Utils::uniform(),Utils::uniform());
    }
};

class Vector3d
{
public:
    int x,y,z;
public:
    Vector3d():x(0),y(0),z(0){}
    Vector3d(int k):x(k),y(k),z(k){}
    Vector3d(int x,int y,int z):x(x),y(y),z(z){}
    ~Vector3d()=default;
public:
    int operator[](unsigned int k) const
    {
        switch(k)
        {
            case 1:return x;
            case 2:return y;
            case 3:return z;
        }
        return 0;
    }
};

class Vector2f
{
public:
    double x,y;
public:
    Vector2f():x(0),y(0){}
    Vector2f(double k):x(k),y(k){}
    Vector2f(double x,double y):x(x),y(y){}
    ~Vector2f()=default;
public:
    Vector2f operator*(double k) const
    {
        return Vector2f(x*k,y*k);
    }
    Vector2f operator+(const Vector2f &rhx) const
    {
        return Vector2f(x+rhx.x,y+rhx.y);
    }
public:
    double operator[](unsigned int k) const
    {
        switch(k)
        {
            case 1:return x;
            case 2:return y;
        }
        return 0;
    }
public:
    double distance(const Vector2f &rhx) const
    {
        double dx=x-rhx.x;
        double dy=y-rhx.y;
        return sqrt(dx*dx+dy*dy);
    }
};

class Vector2d
{
public:
    int x,y;
public:
    Vector2d():x(0),y(0){}
    Vector2d(int k):x(k),y(k){}
    Vector2d(int x,int y):x(x),y(y){}
    ~Vector2d()=default;
public:
    int operator[](unsigned int k) const
    {
        switch(k)
        {
            case 1:return x;
            case 2:return y;
        }
        return 0;
    }
};

typedef Vector3d Triface;

typedef Vector3f Color;
typedef Vector3f Point;
typedef Vector3f Direction;

typedef Vector2f Coord;

typedef Vector2d Index2D;

const Direction x_dir(1,0,0);
const Direction y_dir(0,1,0);
const Direction z_dir(0,0,1);

#endif
