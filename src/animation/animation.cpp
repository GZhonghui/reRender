#include"animation.h"

void Animation::update_scene(unsigned int frame)
{
    Message::print(MessageType::MESSAGE,"Rebuilding scene.");

    if(!main_scene)
    {
        Message::print(MessageType::ERROR,"Cant update empty scene.");
        return;
    }

    main_scene->clear_scene();

    //Set the init state
    if(frame==1)
    {

    }else
    {
        //Update scene models here

//        main_scene->models[0]->move_global(Direction(0,0,2/300.0));

//        double tx=5*std::sin(1.5*pi+(frame-1)*pi/40.0);
//        double ty=5*std::cos(1.5*pi+(frame-1)*pi/40.0);
//        double tz=0;
//        main_scene->look_at(Point(tx,ty,tz),Point(0));
    }

    main_scene->build_scene();
}
