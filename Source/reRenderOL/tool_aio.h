#ifndef TOOL_AIO_H
#define TOOL_AIO_H

#include <vector>
#include <cstdint>

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#ifdef DEF_TOOL

/*
std::vector<QHBoxLayout*> DeadObjectsH;
std::vector<QVBoxLayout*> DeadObjectsV;
std::vector<QGridLayout*> DeadObjectsG;

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
    for(auto i=DeadObjectsH.begin();i!=DeadObjectsH.end();++i) delete *i; DeadObjectsH.clear();
    for(auto i=DeadObjectsV.begin();i!=DeadObjectsV.end();++i) delete *i; DeadObjectsV.clear();
    for(auto i=DeadObjectsG.begin();i!=DeadObjectsG.end();++i) delete *i; DeadObjectsG.clear();
}
*/

#else

/*
extern std::vector<QHBoxLayout*> DeadObjectsH;
extern std::vector<QVBoxLayout*> DeadObjectsV;
extern std::vector<QGridLayout*> DeadObjectsG;


template <class T>
T* DL(T* P);

void DL_Clear();
*/

#endif // DEF_TOOL

/*
inline QHBoxLayout* DL(QHBoxLayout* P) { DeadObjectsH.push_back(P); return P; }
inline QVBoxLayout* DL(QVBoxLayout* P) { DeadObjectsV.push_back(P); return P; }
inline QGridLayout* DL(QGridLayout* P) { DeadObjectsG.push_back(P); return P; }
*/

#endif // TOOL_AIO_H
