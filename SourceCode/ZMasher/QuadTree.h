#pragma once
#include <Math/ZMVector3.h>
#include <DataStructures/GrowArray.h>
#include <SphereCollisionComponentManager.h>
#include <AABBComponentManager.h>
#include <GameObject.h>

#define QT_MAX_DEPTH 3
#define QT_ARGSLIST_SIZE 1024

class QuadTree
{
private:

	struct SphereArgs
	{
		GrowArray<GameObject, GO_ARRAY_PARAMS> m_Spheres;

		GrowArray<float, int, QT_ARGSLIST_SIZE> x;
		GrowArray<float, int, QT_ARGSLIST_SIZE> y;
		GrowArray<float, int, QT_ARGSLIST_SIZE> z;
		GrowArray<float, int, QT_ARGSLIST_SIZE> rad;
		inline void ClearAll()
		{
			x.RemoveAll();
			y.RemoveAll();
			z.RemoveAll();
			rad.RemoveAll();
		}
		inline void RemoveCyclic(int index)
		{
			m_Spheres.RemoveCyclic(index);

			if (x.Size() >0)
			{
				x.RemoveCyclic(index);
				y.RemoveCyclic(index);
				z.RemoveCyclic(index);
				rad.RemoveCyclic(index);
			}
		}

	} m_SphereArgs;

	struct AABBArgs
	{
		GrowArray<GameObject, GO_ARRAY_PARAMS> m_AABBs;

		GrowArray<float, int, QT_ARGSLIST_SIZE> aabbx_min;
		GrowArray<float, int, QT_ARGSLIST_SIZE> aabby_min;
		GrowArray<float, int, QT_ARGSLIST_SIZE> aabbz_min;
		GrowArray<float, int, QT_ARGSLIST_SIZE> aabbx_max;
		GrowArray<float, int, QT_ARGSLIST_SIZE> aabby_max;
		GrowArray<float, int, QT_ARGSLIST_SIZE> aabbz_max;

		inline void ClearAll()
		{
			aabbx_min.RemoveAll();
			aabby_min.RemoveAll();
			aabbz_min.RemoveAll();
			aabbx_max.RemoveAll();
			aabby_max.RemoveAll();
			aabbz_max.RemoveAll();
		}

		inline void RemoveCyclic(int index)
		{
			m_AABBs.RemoveCyclic(index);

			if (aabbx_min.Size() > 0)
			{
				aabbx_min.RemoveCyclic(index);
				aabby_min.RemoveCyclic(index);
				aabbz_min.RemoveCyclic(index);
				aabbx_max.RemoveCyclic(index);
				aabby_max.RemoveCyclic(index);
				aabbz_max.RemoveCyclic(index);
			}
		}

	} m_AABBArgs;

public:
	QuadTree(const ZMasher::Vector3f& center, float halfWidth, QuadTree* parent = nullptr, int depth = 0);
	~QuadTree();

	void InsertObject(GameObject object);
	void TestCollisions(SphereArgs* spheres = nullptr, AABBArgs* aabbs = nullptr);

	static QuadTree* BuildQuadTree(const ZMasher::Vector3f& center, float halfWidth, QuadTree* parent = nullptr, int depth = 0);

	void ValidateObjects();
private:
	bool Ascend(GameObject object);

	float GetRadius(GameObject object);
	void SphereVsSpheres(class SphereArgs* spheres = nullptr);
	void SphereVsAABBs(class SphereArgs* spheres = nullptr);
	void PopulateArgs();
	void TestCollisionsRec(QuadTree* root, QuadTree* child);


	bool Inside(const class ZMasher::Vector4f& pos);
	enum eStraddle
	{
		First,
		Second,
		Third,
		Fourth,
		Straddling = -1
	};

	int Straddle(GameObject object); // -1 straddle
	const int straddling = -1;
	void AddObject(GameObject object);

	QuadTree* Root();

	QuadTree* m_Parent;
	QuadTree* m_Children[4];

	const ZMasher::Vector3f m_Center;
	const int m_Depth; // 0 is root
	const float m_HalfWidth;

	const int m_Colour;
};