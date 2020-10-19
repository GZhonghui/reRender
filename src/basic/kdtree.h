#ifndef KD_TREE_H
#define KD_TREE_H

#include<vector>

#include"../math/vector.h"

#include"message.h"

class KDTree;

class KDTreeNode
{
protected:
    Point position;
    KDTreeNode *left_node;
    KDTreeNode *right_node;
public:
    KDTreeNode():left_node(nullptr),right_node(nullptr){}
    KDTreeNode(const Point &position,
        KDTreeNode *left_node=nullptr,KDTreeNode *right_node=nullptr):
        position(position),left_node(left_node),right_node(right_node){}
    ~KDTreeNode()=default;
public:
    friend class KDTree;
};

class KDTree
{
protected:
    int dimension;
    KDTreeNode *root;
public:
    KDTree():dimension(3),root(nullptr){}
    ~KDTree()
    {
        destroy();
    }
protected:
    void clear_node(KDTreeNode *now);
    void build_range(std::vector<Point> &points,int l,int r,int now_dimension,KDTreeNode **place_node);
public:
    void build_from(std::vector<Point> &points);
    void destroy();
};

#endif