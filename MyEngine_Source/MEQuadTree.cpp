#include "MEQuadTree.h"


namespace ME
{
	QuadTree::QuadTree(int level, math::Vector2 boundsMin, math::Vector2 boundsMax) : mLevel(level), mBoundsMin(boundsMin), mBoundsMax(boundsMax)
	{
		for (int i = 0; i < 4; i++)
		{
			mNodes[i] = nullptr;
		}
	}

	QuadTree::~QuadTree()
	{
		Clear();
	}

	void QuadTree::Clear()
	{
		mColliders.clear();
		for (int i = 0; i < 4; i++)
		{
			if (mNodes[i] != nullptr)
			{
				mNodes[i]->Clear();
				delete mNodes[i];
				mNodes[i] = nullptr;
			}
		}
	}

	void QuadTree::Split()
	{

	}


	int QuadTree::GetIndex(Collider* col)
	{

	}

	void QuadTree::Insert(Collider* col)
	{

	}


	std::vector<Collider*> QuadTree::Retrieve(std::vector<Collider*>& returnObjects, Collider* col)
	{

	}

}