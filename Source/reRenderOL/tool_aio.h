#ifndef TOOL_AIO_H
#define TOOL_AIO_H

#include <vector>

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#ifdef DEF_TOOL

std::vector<QLayout*> DeadObjects;

template <class T>
T* DL(T* P)
{
    DeadObjects.push_back(P);
    return P;
}

template QHBoxLayout* DL<QHBoxLayout>(QHBoxLayout*);
template QVBoxLayout* DL<QVBoxLayout>(QVBoxLayout*);
template QGridLayout* DL<QGridLayout>(QGridLayout*);

void DL_Clear()
{
    for(auto i=DeadObjects.begin();i!=DeadObjects.end();++i)
    {
        delete *i;
    }
    DeadObjects.clear();
}

#else

extern std::vector<QObject*> DeadObjects;

template <class T>
T* DL(T* P);

void DL_Clear();

#endif // DEF_TOOL

#endif // TOOL_AIO_H
