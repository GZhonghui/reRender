#include"n_search.h"

void NSearch2D::update()
{
    data_map.clear();

    double dx=max_limit.x-min_limit.x;
    x_cnts=dx/block_size;
    if(x_cnts*block_size<dx)
    {
        x_cnts+=1;
    }

    double dy=max_limit.y-min_limit.y;
    y_cnts=dy/block_size;
    if(y_cnts*block_size<dy)
    {
        y_cnts+=1;
    }
}

int NSearch2D::get_id(const Coord &center) const
{
    Index2D index;
    get_index(center,index);

    return get_id(index);
}

int NSearch2D::get_id(const Index2D &index) const
{
    return (index.y-1)*x_cnts+index.x;
}

void NSearch2D::get_index(int id,Index2D &index) const
{
    index.x=id%x_cnts;
    index.y=id/x_cnts+1;
}

void NSearch2D::get_index(const Coord &center,Index2D &index) const
{
    double dx=center.x-min_limit.x;
    int x_id=dx/block_size;
    if(x_id*block_size+min_limit.x<center.x)
    {
        x_id+=1;
    }
    
    double dy=center.y-min_limit.y;
    int y_id=dy/block_size;
    if(y_id*block_size+min_limit.y<center.y)
    {
        y_id+=1;
    }

    index=Index2D(x_id,y_id);
}

bool NSearch2D::inside(const Coord &center) const
{
    if(center.x<min_limit.x||center.y<min_limit.y) return false;
    if(center.x>max_limit.x||center.y>max_limit.y) return false;

    return true;
}

void NSearch2D::insert(const Coord &center)
{
    if(!inside(center))
    {
        Message::print(MessageType::WARNING,"Out of range.");

        return;
    }

    int id=get_id(center);
    if(!data_map.count(id))
    {
        data_map[id]=std::vector<Coord>();
    }
    data_map[id].push_back(center);
}

void NSearch2D::search(const Coord &center,double r,std::vector<Coord> *result)
{
    Index2D pos_index;
    get_index(center,pos_index);

    int offset=r/block_size;
    if(offset*block_size<r)
    {
        offset+=1;
    }

    int x_start=pos_index.x-offset,x_end=pos_index.x+offset;
    int y_start=pos_index.y-offset,y_end=pos_index.y+offset;

    for(int x=x_start;x<=x_end;x+=1)
    {
        for(int y=y_start;y<=y_end;y+=1)
        {
            int now_id=get_id(Index2D(x,y));

            for(auto i=data_map[now_id].begin();i!=data_map[now_id].end();++i)
            {
                if(i->distance(center)<r)
                {
                    result->push_back(*i);
                }
            }
        }
    }
}