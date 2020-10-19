#include"kdtree.h"

//Local function
bool cmp_x(const Point &A,const Point &B)
{
    return A.x<B.x;
}

//Local function
bool cmp_y(const Point &A,const Point &B)
{
    return A.y<B.y;
}

//Local function
bool cmp_z(const Point &A,const Point &B)
{
    return A.z<B.z;
}

void KDTree::clear_node(KDTreeNode *now)
{
    if(now)
    {
        if(now->left_node) clear_node(now->left_node);
        if(now->right_node) clear_node(now->right_node);

        delete now;
    }
}

void KDTree::build_range(std::vector<Point> &points,int l,int r,
    int now_dimension,KDTreeNode **place_node)
{
    if(l>r) return;
    
    if(l==r)
    {
        *place_node=new KDTreeNode(points[l+1]);
        return;
    }

    auto l_index=points.begin()+l-1;
    auto r_index=points.begin()+r-1;

    bool (*sort_functions[3])(const Point &,const Point &)={cmp_x,cmp_y,cmp_z};

    sort(l_index,r_index,sort_functions[now_dimension-1]);

    now_dimension=now_dimension+1;
    if(now_dimension>dimension) now_dimension=1;

    int mid=(l+r)>>1;
    *place_node=new KDTreeNode(points[mid]);

    build_range(points,l,mid-1,now_dimension,&(*place_node)->left_node);
    build_range(points,mid+1,r,now_dimension,&(*place_node)->right_node);
}

void KDTree::build_from(std::vector<Point> &points)
{
    int node_count=points.size();

    build_range(points,1,node_count,1,&root);
}

void KDTree::destroy()
{
    if(root)
    {
        clear_node(root);
        root=nullptr;
    }
}