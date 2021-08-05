#include"BVH.h"

void BVH::BuildNode(std::shared_ptr<BVHNode>& Now, int l, int r)
{
	if (l > r)
	{
		//Set to nullptr
		Now.reset();
		return;
	}

	if (l == r)
	{
		Now = std::make_shared<BVHNode>();
		Now->m_Object = (*m_ObjectsPtr)[l = r];
		Now->m_Object->GenerateBox(Now->m_Box);

		Now->m_Left.reset();
		Now->m_Right.reset();

		return;
	}

	auto SortX = [](std::shared_ptr<Object> A, std::shared_ptr<Object> B)->bool
	{
		return A->Center().x() < B->Center().x();
	};

	auto SortY = [](std::shared_ptr<Object> A, std::shared_ptr<Object> B)->bool
	{
		return A->Center().y() < B->Center().y();
	};

	auto SortZ = [](std::shared_ptr<Object> A, std::shared_ptr<Object> B)->bool
	{
		return A->Center().z() < B->Center().z();
	};

	using std::min;
	using std::max;

	auto SortWay = 3;

	double x1 = inf, y1 = inf, z1 = inf;
	double x2 = -1, y2 = -1, z2 = -1;
	for (int i = l; i <= r; i += 1)
	{
		Point face = (*m_ObjectsPtr)[i]->Center();
		x1 = min(x1, face.x());
		x2 = max(x2, face.x());
		y1 = min(y1, face.y());
		y2 = max(y2, face.y());
		z1 = min(z1, face.z());
		z2 = max(z2, face.z());
	}

	double xx = x2 - x1, yy = y2 - y1, zz = z2 - z1;
	if (xx > yy)
	{
		if (xx > zz) SortWay = 1;
		//Default
		//else SortWay = 3;
	}
	else
	{
		if (yy > zz) SortWay = 2;
		//Default
		//else SortWay = 3;
	}

	//Will do Change to Objects in Scene
	auto array = m_ObjectsPtr->begin();
	switch (SortWay)
	{
	case 1:
	{
		std::sort(array + l, array + r + 1, SortX);
	}
	break;

	case 2:
	{
		std::sort(array + l, array + r + 1, SortY);
	}
	break;

	case 3:
	{
		std::sort(array + l, array + r + 1, SortZ);
	}
	break;

	default: break;
	}

	int mid = (l + r) >> 1;
	Now = std::make_shared<BVHNode>();
	BuildNode(Now->m_Left, l, mid);
	BuildNode(Now->m_Right, mid + 1, r);
	Now->m_Box = BoundingBox(Now->m_Left->m_Box, Now->m_Right->m_Box);
}

bool BVH::HitNode(std::shared_ptr<BVHNode> Now, const Ray& ray, Intersection& HitRes)
{
	if (!Now) return false;

	if (Now->m_Object)
	{
		Intersection realHit;
		if (Now->m_Object->Intersect(ray, realHit))
		{
			if (hitNear < realHit.HitDistance && realHit.HitDistance < hitFar)
			{
				HitRes = realHit;
				return true;
			}
		}
		return false;
	}

	Intersection bestHit;

	if (Now->m_Left && Now->m_Left->m_Box.Hit(ray))
	{
		HitNode(Now->m_Left, ray, bestHit);
	}

	if (Now->m_Right && Now->m_Right->m_Box.Hit(ray))
	{
		Intersection ThisHit;
		if (HitNode(Now->m_Right, ray, ThisHit))
		{
			if (ThisHit.HitDistance < bestHit.HitDistance)
			{
				bestHit = ThisHit;
			}
		}
	}

	HitRes = bestHit;
	return HitRes.Hit;
}

bool BVH::Build()
{
	Clear();

	BuildNode(m_Root, 0, static_cast<int>(m_ObjectsPtr->size() - 1));

	return true;
}

bool BVH::Hit(const Ray& ray, Intersection& HitRes)
{
	if (!m_Root)
	{
		return false;
	}
	
	return HitNode(m_Root, ray, HitRes);
}

bool BVH::Clear()
{
	if (m_Root)
	{
		m_Root.reset();
	}

	return true;
}