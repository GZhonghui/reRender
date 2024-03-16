#ifndef N_SEARCH_H
#define N_SEARCH_H

#include<unordered_set>
#include<unordered_map>
#include<vector>

#include"../basic/message.h"

#include"../math/vector.h"

class NSearch2D
{
protected:
    Coord min_limit;
    Coord max_limit;
    double block_size;
protected:
    int x_cnts;
    int y_cnts;
    std::unordered_map<int,std::vector<Coord> > data_map;
public:
    NSearch2D(const Coord &min_limit,const Coord &max_limit,double block_size):
        min_limit(min_limit),max_limit(max_limit),block_size(block_size)
    {
        update();
    }
    ~NSearch2D()=default;
protected:
    void update();
    bool inside(const Coord &center) const;
protected:
    int get_id(const Coord &center) const;
    int get_id(const Index2D &index) const;
    void get_index(int id,Index2D &index) const;
    void get_index(const Coord &center,Index2D &index) const;
public:
    void insert(const Coord &center);
    void search(const Coord &center,double r,std::vector<Coord> *result);
};

class NSearch3D
{
public:
    NSearch3D()=default;
    ~NSearch3D()=default;
public:
    void search();
};

#endif