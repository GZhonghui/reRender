#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

class PhysicsObject
{
public:
    PhysicsObject()=default;
    virtual ~PhysicsObject()=default;
};

class PhysicsPoint:public PhysicsObject
{
public:
    PhysicsPoint()=default;
    virtual ~PhysicsPoint()=default;
};

#endif