#ifndef BASIC_MATH_H
#define BASIC_MATH_H

#include<cmath>

#include"../basic/global.h"

const double eps=1e-8;
const double inf=1e18;
const double pi=acos(-1.0);

class Utils
{
public:
    Utils()=default;
    ~Utils()=default;
public:
    static double uniform()
    {
        static int rand_limit=10240;
        return 1.0*(rand()%rand_limit)/(rand_limit-1);
    }
    static double deg2rad(double deg)
    {
        return deg*pi/180;
    }
    static int range(int l,int r)
    {
        int length=r-l+1;
        return rand()%length+l;
    }
};

#endif