#include"bvh.h"

//Local function
bool sort_x(Object *A,Object *B)
{
    return A->center().x<B->center().x;
}

//Local function
bool sort_y(Object *A,Object *B)
{
    return A->center().y<B->center().y;
}

//Local function
bool sort_z(Object *A,Object *B)
{
    return A->center().z<B->center().z;
}

bool BVH::hit_node(BVHNode *now,const Ray &ray,Intersection &hit_result) const
{
    if(!now) return false;
    if(now->object)
    {
        return now->object->intersect(ray,hit_result);
    }

    bool happened=false;
    if(now->left&&now->left->bounding_box.hit(ray))
    {
        happened|=hit_node(now->left,ray,hit_result);
    }
    if(now->right&&now->right->bounding_box.hit(ray))
    {
        Intersection right_hit;
        if(hit_node(now->right,ray,right_hit))
        {
            if(!happened||right_hit.distance<hit_result.distance)
            {
                hit_result=right_hit;
            }
            happened=true;
        }
    }
    return happened;
}

void BVH::build_node(BVHNode **now,int l,int r)
{
    if(l>r)
    {
        (*now)=nullptr;
        return;
    }
    if(l==r)
    {
        (*now)=new BVHNode((*faces)[l],nullptr,nullptr);
        (*faces)[l]->generate_bbox((*now)->bounding_box);
        return;
    }

    using std::min;
    using std::max;
    int sort_way=0;
    double x1=inf,y1=inf,z1=inf;
    double x2=-1,y2=-1,z2=-1;
    for(int i=l;i<=r;i+=1)
    {
        Point face=(*faces)[i]->center();
        x1=min(x1,face.x);
        x2=max(x2,face.x);
        y1=min(y1,face.y);
        y2=max(y2,face.y);
        z1=min(z1,face.z);
        z2=max(z2,face.z);
    }
    double xx=x2-x1,yy=y2-y1,zz=z2-z1;
    if(xx>yy)
    {
        if(xx>zz) sort_way=1;
        else sort_way=3;
    }else
    {
        if(yy>zz) sort_way=2;
        else sort_way=3;
    }

    auto array=faces->begin();
    std::sort(array+l,array+r+1,sort_way==1?sort_x:
        (sort_way==2?sort_y:sort_z));

    int mid=(l+r)>>1;
    (*now)=new BVHNode();
    build_node(&((*now)->left),l,mid);
    build_node(&((*now)->right),mid+1,r);
    (*now)->bounding_box=BBox((*now)->left->bounding_box,
        (*now)->right->bounding_box);
}

void BVH::clear_node(BVHNode *now)
{
    if(now)
    {
        clear_node(now->left);
        clear_node(now->right);
        delete now;
    }
}

bool BVH::hit(const Ray &ray,Intersection &hit_result) const
{
    if(!root) return false;
    return hit_node(root,ray,hit_result);
}

void BVH::build(std::vector<Object *> *faces)
{
    this->faces=faces;
    int face_count=(int)faces->size();

    char build_bvh_message[message_size];
    sprintf(build_bvh_message,"Build BVH with %d faces.",face_count);
    Message::print(MessageType::MESSAGE,build_bvh_message);

    build_node(&root,0,face_count-1);
}

void BVH::clear()
{
    clear_node(root);
    root=nullptr;
}
