#ifndef GLOBAL_H
#define GLOBAL_H

#include<algorithm>
#include<ctime>

#include"render/render.h"

extern "C"
{
    void py_init_lib();
    void py_set_args(int *int_args,double *double_args);
    void py_generate(double *pixels);
    void py_clear_lib();
}

#endif
