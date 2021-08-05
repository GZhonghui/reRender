#pragma once

#include<vector>
#include<memory>

#include"Ray.h"
#include"Object.h"
#include"Intersection.h"
#include"MathAllInOne.h"
#include"ToolAllInOne.h"

class BVHNode
{
protected:
    std::shared_ptr<Object> m_Object;
    std::shared_ptr<BVHNode> m_Left;
    std::shared_ptr<BVHNode> m_Right;
    BoundingBox m_Box;

public:
    BVHNode() = default;
    ~BVHNode() = default;

public:
    friend class BVH;
};

class BVH
{
public:
	BVH():m_ObjectsPtr(nullptr), m_Root(nullptr){}
	BVH(std::vector<std::shared_ptr<Object>>* ObjectsPtr):
		m_ObjectsPtr(ObjectsPtr){}

	~BVH() = default;

protected:
	std::vector<std::shared_ptr<Object>>* m_ObjectsPtr;
    std::shared_ptr<BVHNode> m_Root;

protected:
    void BuildNode(std::shared_ptr<BVHNode>& Now, int l, int r);
    bool HitNode(std::shared_ptr<BVHNode> Now, const Ray& ray, Intersection& HitRes);

public:
	bool Build();
    bool Hit(const Ray& ray, Intersection& HitRes);
    bool Clear();
};