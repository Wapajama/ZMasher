#include "QuadTree.h"
#include <GameObjectManager.h>
#include <SphereCollisionComponentManager.h>
#include <AABBComponentManager.h>
#include <ZMasherGfxDX11\ZMModelFactory.h>

static void ZombieCollisionFeedback(ZMasher::Matrix44f* __restrict transformA, ZMasher::Matrix44f* __restrict transformB, const float diff)
{
	ZMasher::Vector4f dir = (transformA->GetTranslation() - transformB->GetTranslation()).GetNormal() * (diff * 0.5f);
	dir.y = 0; // don't want them to get pushed over or under
	transformA->SetTranslation(transformA->GetTranslation() - dir);
}

static bool SphereVsSphereTest(float r_a, float r_b,
							   const ZMasher::Vector3f& __restrict pos_a, const ZMasher::Vector3f& __restrict pos_b)
{
	using namespace ZMasher;
	Vector3f d = pos_a - pos_b;
	const float dist2 = Dot(d, d);
	float radSum = r_a + r_b;
	return dist2 <= radSum * radSum;
}

static float SqDistToAABB(const ZMasher::Vector4f& __restrict pos_a,
						  const float* r,
						  const ZMasher::Vector4f& __restrict pos_aabb)
{
	float p[3] = { pos_a.x, pos_a.y, pos_a.z };
	float b[3] = { pos_aabb.x, pos_aabb.y, pos_aabb.z };
	float sqDist = 0.f;
	for (short i = 0; i < 3; i++)
	{
		float v = p[i];
		if (v < (b[i] - r[i])) sqDist += ((b[i] - r[i]) - v) * ((b[i] - r[i]) - v);
		if (v > (b[i] + r[i])) sqDist += (v - (b[i] + r[i])) * (v - (b[i] + r[i]));
	}
	return sqDist;
}

static bool SphereVsAABBTest(float r_a, float* __restrict r_b,
							 const ZMasher::Vector4f& pos_sphere, const ZMasher::Vector4f& pos_aabb, float& __restrict sqDistRef)
{
	const float sqDist = SqDistToAABB(pos_sphere, r_b, pos_aabb);
	sqDistRef = sqDist;
	return sqDist <= r_a * r_a;
}

#define DRAW_DEBUG_LINES
static void SphereCollision(GameObject object_a, GameObject object_b)
{
	if (!GameObjectManager::Instance()->Alive(object_a) ||
		!GameObjectManager::Instance()->Alive(object_b))
	{
		return;
	}
	ZMasher::Matrix44f* transform_a = GameObjectManager::Instance()->TransformManager()->GetTransform(object_a);
	ZMasher::Matrix44f* transform_b = GameObjectManager::Instance()->TransformManager()->GetTransform(object_b);

	MomentumComponent* mom_a = GameObjectManager::Instance()->MomentumCompManager()->GetComponent(object_a);
	MomentumComponent* mom_b = GameObjectManager::Instance()->MomentumCompManager()->GetComponent(object_b);

	SphereCollisionComponent* sphere_a = GameObjectManager::Instance()->SphereCollisionCompManager()->GetComponent(object_a);
	SphereCollisionComponent* sphere_b = GameObjectManager::Instance()->SphereCollisionCompManager()->GetComponent(object_b);

	ZMasher::Vector3f diffv = transform_a->GetTranslation().ToVector3f() - transform_b->GetTranslation().ToVector3f();
	const float diff = diffv.Length() - (sphere_a->m_Radius + sphere_b->m_Radius);

	ZombieCollisionFeedback(transform_a, transform_b, diff);
	ZombieCollisionFeedback(transform_b, transform_a, diff);

	if (sphere_a->m_CollisionCallback)
	{
		(sphere_a->m_CollisionCallback)({ sphere_a, sphere_b, mom_a, mom_b });
	}
	if (sphere_b->m_CollisionCallback)
	{
		(sphere_b->m_CollisionCallback)({ sphere_b, sphere_a, mom_b, mom_a });
	}
}

static void SphereAABBCallback(GameObject sphere_obj, GameObject aabb_obj)
{
	if (!GameObjectManager::Instance()->Alive(sphere_obj) ||
		!GameObjectManager::Instance()->Alive(aabb_obj))
	{
		return;
	}
	SphereCollisionComponent* sphere = GameObjectManager::Instance()->SphereCollisionCompManager()->GetComponent(sphere_obj);
	AABBCollisionComponent* aabb = GameObjectManager::Instance()->AABBCompManager()->GetComponent(aabb_obj);

	if (sphere->m_CollisionCallback)
	{
		(sphere->m_CollisionCallback)({ sphere, nullptr, nullptr, nullptr, aabb, nullptr });
	}
}

QuadTree::QuadTree(const ZMasher::Vector3f& center,
				   float halfWidth,
				   QuadTree* parent,
				   int depth) :
	m_Center(center),
	m_HalfWidth(halfWidth),
	m_Parent(parent),
	m_Depth(depth),
	m_Colour(depth)
{
	for (byte i = 0; i < 4; i++)
	{
		m_Children[i] = nullptr;
	}
}

QuadTree::~QuadTree()
{
}

void QuadTree::InsertObject(GameObject object)
{
	int index = this->Straddle(object);
	if (index != eStraddle::Straddling &&
		m_Children[index])
	{
		m_Children[index]->InsertObject(object);
	}
	else
	{
		this->AddObject(object);
	}
}

//bool QuadTree::Ascend(GameObject object)
//{
//	//QuadTree* parent = this->m_Parent;
//	//QuadTree* last_parent = nullptr;
//	//while (parent)
//	//{
//	//	if (parent->Straddle(object) == eStraddle::Straddling)
//	//	{
//	//		last_parent = parent;
//	//	}
//	//	parent = parent->m_Parent;
//	//}
//
//	//if (last_parent)
//	//{
//	//	last_parent->InsertObject(object);
//	//	return true;
//	//}
//	//return false;
//	//if (parent &&
//	//	parent->Straddle(object) == this->straddling)
//	//{
//	//	parent->Ascend(object);
//	//}
//	//else
//	//{
//	//	m_Parent->InsertObject(object);
//	//}
//}

void QuadTree::AddObject(GameObject object)
{
	if (!GameObjectManager::Instance()->Alive(object))
	{
		return;
	}

	SphereCollisionComponent* sphere = GameObjectManager::Instance()->SphereCollisionCompManager()->GetComponent(object);
	if (sphere)
	{
		m_SphereArgs.m_Spheres.Add(object);
	}
	AABBCollisionComponent* aabb = GameObjectManager::Instance()->AABBCompManager()->GetComponent(object);
	if (aabb)
	{
		m_AABBArgs.m_AABBs.Add(object);
	}
}

QuadTree* QuadTree::BuildQuadTree(const ZMasher::Vector3f& center, float halfWidth, QuadTree* parent, int depth)
{
	static int colour = 0;
	if (depth >= QT_MAX_DEPTH)
	{
		return nullptr;
	}
	QuadTree* node = new QuadTree(center, halfWidth, parent, depth);
	//node->m_Colour = colour;
	++colour;
	colour = colour % N_COLOURS;
	ZMasher::Vector3f offset = { 0,0,0 };
	float step = halfWidth * 0.5f;
	for (int i = 0; i < 4; i++)
	{
		offset.x = ((i & 1) ? step : -step);
		offset.z = ((i & 2) ? step : -step);
		node->m_Children[i] = BuildQuadTree(center + offset, step, node, depth + 1);
	}
	return node;
}

bool QuadTree::Inside(const class ZMasher::Vector4f& pos)
{
	if (pos.x < m_Center.x + m_HalfWidth &&
		pos.x > m_Center.x - m_HalfWidth &&
		pos.y < m_Center.y + m_HalfWidth &&
		pos.y > m_Center.y - m_HalfWidth &&
		pos.z < m_Center.z + m_HalfWidth &&
		pos.z > m_Center.z - m_HalfWidth)
	{
		return true;
	}
	return false;
}

float QuadTree::GetRadius(GameObject object)
{
	SphereCollisionComponent* sphere = GameObjectManager::Instance()->SphereCollisionCompManager()->GetComponent(object);
	if (sphere)
	{
		return sphere->m_Radius;
	}
	AABBCollisionComponent* aabb = GameObjectManager::Instance()->AABBCompManager()->GetComponent(object);
	if (aabb)
	{
		return sqrt(aabb->r[0] * aabb->r[0] + aabb->r[2] * aabb->r[2]);
	}
	ASSERT(false, "Object is missing a collisioncomponent!");
	return 0.f;
}

int QuadTree::Straddle(GameObject object)
{
	ZMasher::Vector4f pos = GameObjectManager::Instance()->TransformManager()->GetTranslation(object);
	float radius = this->GetRadius(object);
	float d = pos.x - this->m_Center.x;

	int result = 0;

	if (abs(d) <= radius || abs(d) > m_HalfWidth)
	{
		return straddling;
	}
	else if (d > 0.f)
	{
		result |= (1 << 0);
	}

	d = pos.z - this->m_Center.z;

	if (abs(d) <= radius || abs(d) > m_HalfWidth)
	{
		return straddling;
	}
	else if (d > 0.f)
	{
		result |= (1 << 1);
	}

	return result;
}

static inline int StraddleSimdTest(float dx, float dz, float radius, float halfWidth)
{
	int result = 0;
	if (abs(dx) <= radius || abs(dx) > halfWidth)
	{
		return -1;
	}
	else if (dx > 0.f)
	{
		result |= (1 << 0);
	}

	if (abs(dz) <= radius || abs(dz) > halfWidth)
	{
		return -1;
	}
	else if (dz > 0.f)
	{
		result |= (1 << 1);
	}

	return result;
}

void QuadTree::TestCollisions(SphereArgs* spheres, AABBArgs* aabbs)
{
#pragma region DRAWNODE
	const float x = m_HalfWidth + m_Colour;
	const float y = 0;
	const float z = m_HalfWidth + m_Colour;
#ifdef DRAW_DEBUG_LINES
	ZMasher::Vector3f pos = m_Center;
	(ZMModelFactory::Instance()->CreateDebugLine(
		pos + ZMasher::Vector3f(x, y, z),
		pos + ZMasher::Vector3f(x, y, -z),
		(eColour)m_Colour));
	(ZMModelFactory::Instance()->CreateDebugLine(
		pos + ZMasher::Vector3f(x, y, z),
		pos + ZMasher::Vector3f(-x, y, z),
		(eColour)m_Colour));
	(ZMModelFactory::Instance()->CreateDebugLine(
		pos + ZMasher::Vector3f(-x, y, -z),
		pos + ZMasher::Vector3f(x, y, -z),
		(eColour)m_Colour));
	(ZMModelFactory::Instance()->CreateDebugLine(
		pos + ZMasher::Vector3f(-x, y, -z),
		pos + ZMasher::Vector3f(-x, y, z),
		(eColour)m_Colour));
#endif // DRAW_DEBUG_LINES

#pragma endregion

	//this->PopulateArgs();

	this->SphereVsSpheres();
	this->SphereVsAABBs();

	for (int i = 0; i < 4; i++)
	{
		if (m_Children[i])
		{
			m_Children[i]->TestCollisions();
		}
	}

	if (m_Depth < QT_MAX_DEPTH-1)
	{
		this->TestCollisionsRec(this, this);
	}

	this->ValidateObjects();
}

QuadTree* QuadTree::Root()
{
	QuadTree* root = this;
	while (root->m_Parent)
	{
		root = root->m_Parent;
	}
	return root;
}

void QuadTree::ValidateObjects()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_Children[i])
		{
			m_Children[i]->ValidateObjects();
		}
	}
	const float cx = this->m_Center.x;
	const float cz = this->m_Center.z;
	GrowArray<int, short, 512> ascending;
	int speheresSize = m_SphereArgs.m_Spheres.Size();
	if (false)
	{
		int i = 0;
		for (; i < m_SphereArgs.m_Spheres.Size(); i += 4)
		{
			float dx[4] =
			{
				m_SphereArgs.x[i] - cx,
				m_SphereArgs.x[i + 1] - cx,
				m_SphereArgs.x[i + 2] - cx,
				m_SphereArgs.x[i + 3] - cx
			};

			float dz[4] =
			{
				m_SphereArgs.z[i] - cz,
				m_SphereArgs.z[i + 1] - cz,
				m_SphereArgs.z[i + 2] - cz,
				m_SphereArgs.z[i + 3] - cz
			};

			int result[4] =
			{
				StraddleSimdTest(dx[0], dz[0], m_SphereArgs.rad[i + 0], m_HalfWidth),
				StraddleSimdTest(dx[1], dz[1], m_SphereArgs.rad[i + 1], m_HalfWidth),
				StraddleSimdTest(dx[2], dz[2], m_SphereArgs.rad[i + 2], m_HalfWidth),
				StraddleSimdTest(dx[3], dz[3], m_SphereArgs.rad[i + 3], m_HalfWidth)
			};

			for (int j = 0; j < 4; j++)
			{
				if (result[j] == eStraddle::Straddling)
				{
					ascending.Add(i + j);
					//this->Ascend(m_SphereArgs.m_Spheres[i + j]);
				}
			}
		}

		for (; i < m_SphereArgs.m_Spheres.Size(); ++i)
		{
			int index = this->Straddle(m_SphereArgs.m_Spheres[i]);
			if (index == eStraddle::Straddling)
			{
				ascending.Add(i);
				//this->Ascend(m_SphereArgs.m_Spheres[i]);
			}
		}
	}

	if (true)
	{
		for (int i = 0; i < m_SphereArgs.m_Spheres.Size(); i++)
		{
			int straddleRes = this->Straddle(m_SphereArgs.m_Spheres[i]);
			if (straddleRes != eStraddle::Straddling &&
				m_Children[straddleRes])
			{
				m_Children[straddleRes]->InsertObject(m_SphereArgs.m_Spheres[i]);
				ascending.Add(i);
			}
			else if (straddleRes == eStraddle::Straddling)
			{
				QuadTree* tree = Root();

				int parentStraddle = 0;
				parentStraddle = tree->Straddle(m_SphereArgs.m_Spheres[i]);
				while (tree != this && parentStraddle != eStraddle::Straddling)
				{
					if (tree->m_Children[parentStraddle])
					{
						tree = tree->m_Children[parentStraddle];
					}
					else
					{
						break;
					}
					parentStraddle = tree->Straddle(m_SphereArgs.m_Spheres[i]);
				}
				if (tree == this && parentStraddle == eStraddle::Straddling)
				{
					continue;
				}
				if (tree != this)
				{
					tree->InsertObject(m_SphereArgs.m_Spheres[i]);
					ascending.Add(i);
				}
			}
		}

		for (int i = ascending.Size() - 1; i >= 0; --i)
		{
			m_SphereArgs.RemoveCyclic(ascending[i]);
		}

		for (int j = m_SphereArgs.m_Spheres.Size() - 1; j >= 0; --j)
		{
			if (!GameObjectManager::Instance()->Alive(m_SphereArgs.m_Spheres[j]))
			{
				m_SphereArgs.RemoveCyclic(j);
			}
		}
	}
}

void QuadTree::TestCollisionsRec(QuadTree* root, QuadTree* child)
{
	for (int i = 0; i < 4; i++)
	{
		if (child->m_Children[i])
		{
			child->m_Children[i]->TestCollisionsRec(root, child->m_Children[i]);
		}
	}
	if (root != child)
	{
		root->SphereVsSpheres(&child->m_SphereArgs);
		root->SphereVsAABBs(&child->m_SphereArgs);
	}
}

void QuadTree::PopulateArgs()
{
	m_SphereArgs.ClearAll();
	GrowArray<int, short, 512> removing;
	for (int i = 0; i < m_SphereArgs.m_Spheres.Size(); i++)
	{
		if (!GameObjectManager::Instance()->Alive(m_SphereArgs.m_Spheres[i]))
		{
			removing.Add(i);
			continue;
		}
		SphereCollisionComponent* sphere = GameObjectManager::Instance()->SphereCollisionCompManager()->GetComponent(m_SphereArgs.m_Spheres[i]);
		m_SphereArgs.rad.Add(sphere->m_Radius);
		ZMasher::Vector4f t = GameObjectManager::Instance()->TransformManager()->GetTranslation(m_SphereArgs.m_Spheres[i]);
		m_SphereArgs.x.Add(t.x);
		m_SphereArgs.y.Add(t.y);
		m_SphereArgs.z.Add(t.z);

#ifdef DRAW_DEBUG_LINES
		const float x = m_SphereArgs.rad.GetLast();
		const float y = 0;
		const float z = m_SphereArgs.rad.GetLast();
		ZMasher::Vector3f pos = ZMasher::Vector3f(
			m_SphereArgs.x.GetLast(),
			m_SphereArgs.y.GetLast(),
			m_SphereArgs.z.GetLast());
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, z),
			pos + ZMasher::Vector3f(x, y, -z),
			(eColour)m_Colour));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(x, y, z),
			pos + ZMasher::Vector3f(-x, y, z),
			(eColour)m_Colour));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, y, -z),
			pos + ZMasher::Vector3f(x, y, -z),
			(eColour)m_Colour));
		(ZMModelFactory::Instance()->CreateDebugLine(
			pos + ZMasher::Vector3f(-x, y, -z),
			pos + ZMasher::Vector3f(-x, y, z),
			(eColour)m_Colour));
#endif // DRAW_DEBU_LINES


	}

	for (int i = removing.Size() - 1; i >= 0; --i)
	{
		m_SphereArgs.RemoveCyclic(removing[i]);
	}

	removing.RemoveAll();
	m_AABBArgs.ClearAll();
	for (int i = 0; i < m_AABBArgs.m_AABBs.Size(); i++)
	{
		if (!GameObjectManager::Instance()->Alive(m_AABBArgs.m_AABBs[i]))
		{
			removing.Add(i);
			continue;
		}
		ZMasher::Vector4f tran = GameObjectManager::Instance()->TransformManager()->GetTranslation(m_AABBArgs.m_AABBs[i]);
		AABBCollisionComponent* aabb = GameObjectManager::Instance()->AABBCompManager()->GetComponent(m_AABBArgs.m_AABBs[i]);
		m_AABBArgs.aabbx_min.Add(tran.x - aabb->r[0]);
		m_AABBArgs.aabby_min.Add(tran.y - aabb->r[1]);
		m_AABBArgs.aabbz_min.Add(tran.z - aabb->r[2]);
		m_AABBArgs.aabbx_max.Add(tran.x + aabb->r[0]);
		m_AABBArgs.aabby_max.Add(tran.y + aabb->r[1]);
		m_AABBArgs.aabbz_max.Add(tran.z + aabb->r[2]);
	}

	for (int i = removing.Size() - 1; i >= 0; --i)
	{
		m_AABBArgs.m_AABBs.RemoveCyclic(removing[i]);
		m_AABBArgs.RemoveCyclic(removing[i]);
	}
}

void QuadTree::SphereVsSpheres(SphereArgs* spheres)
{
	this->PopulateArgs();
	SphereArgs* args = spheres ? spheres : &m_SphereArgs;
	int i = 0;
	for (; i < m_SphereArgs.m_Spheres.Size(); ++i)
	{
		if (!GameObjectManager::Instance()->Alive(m_SphereArgs.m_Spheres[i]))
		{
			continue;
		}
		const float rA =	m_SphereArgs.rad[i];
		const float posAx = m_SphereArgs.x[i];
		const float posAy = m_SphereArgs.y[i];
		const float posAz = m_SphereArgs.z[i];
		int j = i + 1;
		for (; j + 4 < args->m_Spheres.Size() && args->m_Spheres.Size() > 4; j += 4)
		{
#ifdef DRAW_DEBUG_LINES
			(ZMModelFactory::Instance()->CreateDebugLine(
				ZMasher::Vector3f(posAx, posAy, posAz),
				ZMasher::Vector3f(args->x[j + 0], args->y[j + 0], args->z[j + 0]),
				(eColour)m_Colour));

			(ZMModelFactory::Instance()->CreateDebugLine(
				ZMasher::Vector3f(posAx, posAy, posAz),
				ZMasher::Vector3f(args->x[j + 1], args->y[j + 1], args->z[j + 1]),
				(eColour)m_Colour));

			(ZMModelFactory::Instance()->CreateDebugLine(
				ZMasher::Vector3f(posAx, posAy, posAz),
				ZMasher::Vector3f(args->x[j + 2], args->y[j + 2], args->z[j + 2]),
				(eColour)m_Colour));

			(ZMModelFactory::Instance()->CreateDebugLine(
				ZMasher::Vector3f(posAx, posAy, posAz),
				ZMasher::Vector3f(args->x[j + 3], args->y[j + 3], args->z[j + 3]),
				(eColour)m_Colour));
#endif // DEBUG


#pragma region SIMD Sphere Collision
			float t1[4] = { posAx - args->x[j],
			posAx - args->x[j + 1],
			posAx - args->x[j + 2],
			posAx - args->x[j + 3] };

			t1[0] = t1[0] * t1[0];
			t1[1] = t1[1] * t1[1];
			t1[2] = t1[2] * t1[2];
			t1[3] = t1[3] * t1[3];

			float t2[4] = { posAy - args->y[j],
							posAy - args->y[j + 1],
							posAy - args->y[j + 2],
							posAy - args->y[j + 3] };

			t2[0] = t2[0] * t2[0];
			t2[1] = t2[1] * t2[1];
			t2[2] = t2[2] * t2[2];
			t2[3] = t2[3] * t2[3];

			float t3[4] = { posAz - args->z[j],
							posAz - args->z[j + 1],
							posAz - args->z[j + 2],
							posAz - args->z[j + 3] };

			t3[0] = t3[0] * t3[0];
			t3[1] = t3[1] * t3[1];
			t3[2] = t3[2] * t3[2];
			t3[3] = t3[3] * t3[3];

			t1[0] = t1[0] + t2[0];
			t1[1] = t1[1] + t2[1];
			t1[2] = t1[2] + t2[2];
			t1[3] = t1[3] + t2[3];

			float t4[4] = { rA + args->rad[j],
							rA + args->rad[j + 1],
							rA + args->rad[j + 2],
							rA + args->rad[j + 3] };

			t4[0] = t4[0] * t4[0];
			t4[1] = t4[1] * t4[1];
			t4[2] = t4[2] * t4[2];
			t4[3] = t4[3] * t4[3];

			t2[0] = t4[0] - t3[0];
			t2[1] = t4[1] - t3[1];
			t2[2] = t4[2] - t3[2];
			t2[3] = t4[3] - t3[3];

			bool r[4] = { t1[0] <= t2[0],
							t1[1] <= t2[1],
							t1[2] <= t2[2],
							t1[3] <= t2[3] };
#pragma endregion

			if (r[0])
			{
				SphereCollision(m_SphereArgs.m_Spheres[i], args->m_Spheres[j]);
			}
			if (r[1])
			{
				SphereCollision(m_SphereArgs.m_Spheres[i], args->m_Spheres[j + 1]);
			}
			if (r[2])
			{
				SphereCollision(m_SphereArgs.m_Spheres[i], args->m_Spheres[j + 2]);
			}
			if (r[3])
			{
				SphereCollision(m_SphereArgs.m_Spheres[i], args->m_Spheres[j + 3]);
			}
			if (!GameObjectManager::Instance()->Alive(args->m_Spheres[i]))
			{
				break;
			}
		}
		j -= 4;
		if (j < 0)
		{
			j = 0;
		}
		for (; j < args->m_Spheres.Size(); ++j)
		{
			if (j == i)
			{
				continue;
			}

#ifdef DRAW_DEBUG_LINES
			(ZMModelFactory::Instance()->CreateDebugLine(
				ZMasher::Vector3f(posAx, posAy, posAz),
				ZMasher::Vector3f(args->x[j + 0], args->y[j + 0], args->z[j + 0]),
				(eColour)m_Colour));
#endif // DRAW_DEBUG_LINES


			const bool result = SphereVsSphereTest(rA, args->rad[j],
												   {posAx,
													posAy,
													posAz },
												   {
													args->x[j],
												    args->y[j],
												    args->z[j]});
			if (result)
			{
				SphereCollision(m_SphereArgs.m_Spheres[i], args->m_Spheres[j]);
			}
		}
	}

	for (; i < m_SphereArgs.m_Spheres.Size(); i++)
	{
		for (int j = 0; j < args->m_Spheres.Size(); ++j)
		{
			if (i == j)
			{
				continue;
			}
			const bool result = SphereVsSphereTest(m_SphereArgs.rad[i], args->rad[j],
												   {m_SphereArgs.x[i],
													m_SphereArgs.y[i],
													m_SphereArgs.z[i] },
												   { args->x[j],
												   args->y[j],
												   args->z[j] });
			if (result)
			{
				SphereCollision(m_SphereArgs.m_Spheres[i], args->m_Spheres[j]);
			}
		}
	}
}

void QuadTree::SphereVsAABBs(SphereArgs* spheres)
{
	this->PopulateArgs();
	SphereArgs* args = spheres ? spheres : &m_SphereArgs;
	for (int i = 0; i < args->m_Spheres.Size(); i++)
	{
		int j = 0;
		for (; j < m_AABBArgs.m_AABBs.Size() && args->m_Spheres.Size() > 4; j += 4)
		{

#pragma region AABB Simd collision

			float minx[4] = {
				m_AABBArgs.aabbx_min[j],
				m_AABBArgs.aabbx_min[j + 1],
				m_AABBArgs.aabbx_min[j + 2],
				m_AABBArgs.aabbx_min[j + 3] };

			float tx[4] = {
				max(minx[0], args->x[i]),
				max(minx[1], args->x[i]),
				max(minx[2], args->x[i]),
				max(minx[3], args->x[i]) };

			float miny[4] = {
				m_AABBArgs.aabby_min[j],
				m_AABBArgs.aabby_min[j + 1],
				m_AABBArgs.aabby_min[j + 2],
				m_AABBArgs.aabby_min[j + 3] };

			float ty[4] = {
				max(miny[0], args->y[i]),
				max(miny[1], args->y[i]),
				max(miny[2], args->y[i]),
				max(miny[3], args->y[i]) };

			float minz[4] = {
				m_AABBArgs.aabby_min[j],
				m_AABBArgs.aabby_min[j + 1],
				m_AABBArgs.aabby_min[j + 2],
				m_AABBArgs.aabby_min[j + 3] };

			float tz[4] = {
				max(minz[0], args->z[i]),
				max(minz[1], args->z[i]),
				max(minz[2], args->z[i]),
				max(minz[3], args->z[i]) };

			float maxx[4] = {
				m_AABBArgs.aabbx_max[j],
				m_AABBArgs.aabbx_max[j + 1],
				m_AABBArgs.aabbx_max[j + 2],
				m_AABBArgs.aabbx_max[j + 3] };

			tx[0] = min(maxx[0], args->x[i]);
			tx[1] = min(maxx[1], args->x[i]);
			tx[2] = min(maxx[2], args->x[i]);
			tx[3] = min(maxx[3], args->x[i]);

			float dx[4] = {
				args->x[i] - tx[0],
				args->x[i] - tx[1],
				args->x[i] - tx[2],
				args->x[i] - tx[3] };

			dx[0] = dx[0] * dx[0];
			dx[1] = dx[1] * dx[1];
			dx[2] = dx[2] * dx[2];
			dx[3] = dx[3] * dx[3];

			float maxy[4] = {
				m_AABBArgs.aabby_max[j],
				m_AABBArgs.aabby_max[j + 1],
				m_AABBArgs.aabby_max[j + 2],
				m_AABBArgs.aabby_max[j + 3] };

			ty[0] = min(maxy[0], args->y[i]);
			ty[1] = min(maxy[1], args->y[i]);
			ty[2] = min(maxy[2], args->y[i]);
			ty[3] = min(maxy[3], args->y[i]);

			float dy[4] = {
				args->y[i] - ty[0],
				args->y[i] - ty[1],
				args->y[i] - ty[2],
				args->y[i] - ty[3] };

			dy[0] = dy[0] * dy[0];
			dy[1] = dy[1] * dy[1];
			dy[2] = dy[2] * dy[2];
			dy[3] = dy[3] * dy[3];

			float maxz[4] = {
				m_AABBArgs.aabbz_max[j],
				m_AABBArgs.aabbz_max[j + 1],
				m_AABBArgs.aabbz_max[j + 2],
				m_AABBArgs.aabbz_max[j + 3] };

			tz[0] = min(maxz[0], args->z[i]);
			tz[1] = min(maxz[1], args->z[i]);
			tz[2] = min(maxz[2], args->z[i]);
			tz[3] = min(maxz[3], args->z[i]);

			float dz[4] = {
				args->z[i] - tz[0],
				args->z[i] - tz[1],
				args->z[i] - tz[2],
				args->z[i] - tz[3] };

			dz[0] = dz[0] * dz[0];
			dz[1] = dz[1] * dz[1];
			dz[2] = dz[2] * dz[2];
			dz[3] = dz[3] * dz[3];

			float r2[4] = {
				args->rad[i] * args->rad[i],
				args->rad[i] * args->rad[i],
				args->rad[i] * args->rad[i],
				args->rad[i] * args->rad[i] };

			float t1[4] = {
				dx[0] + dy[0],
				dx[1] + dy[1],
				dx[2] + dy[2],
				dx[3] + dy[3]
			};

			float t2[4] = {
				r2[0] - dz[0],
				r2[0] - dz[1],
				r2[0] - dz[2],
				r2[0] - dz[3]
			};

			bool result[4] = {
				t1[0] <= t2[0],
				t1[1] <= t2[1],
				t1[2] <= t2[2],
				t1[3] <= t2[3]
			};
#pragma endregion

			for (byte index = 0; i < 4; i++)
			{
				if (result[index])
				{
					ZMasher::Matrix44f* transform_a = GameObjectManager::Instance()->TransformManager()->GetTransform(args->m_Spheres[i]);
					ZMasher::Matrix44f* transform_b = GameObjectManager::Instance()->TransformManager()->GetTransform(m_AABBArgs.m_AABBs[j + index]);
					// Sphere and box are intersecting, separate them.
					float r[3] = { (m_AABBArgs.aabbx_min[j + index] - m_AABBArgs.aabbx_max[j + index]) * 0.5f,
									(m_AABBArgs.aabby_min[j + index] - m_AABBArgs.aabby_max[j + index]) * 0.5f,
									(m_AABBArgs.aabbz_min[j + index] - m_AABBArgs.aabbz_max[j + index]) * 0.5f };
					const float sqDist = SqDistToAABB(transform_a->GetTranslation(), r, transform_b->GetTranslation());
					ZombieCollisionFeedback(transform_a, transform_b, sqrt(sqDist) - args->rad[i]);
					SphereCollisionComponent* sphere = GameObjectManager::Instance()->SphereCollisionCompManager()->GetComponent(args->m_Spheres[i]);
				}
			}
			if (!GameObjectManager::Instance()->Alive(args->m_Spheres[i]))
			{
				break;
			}
		}
		j -= 4;
		if (j < 0)
		{
			j = 0;
		}
		for (; j < m_AABBArgs.m_AABBs.Size(); ++j)
		{
			AABBCollisionComponent& aabb = *GameObjectManager::Instance()->AABBCompManager()->GetComponent(m_AABBArgs.m_AABBs[j]);
			GameObject aabb_object = aabb.m_GameObject;
			if (!GameObjectManager::Instance()->Alive(aabb_object))
			{
				continue;
			}
			if (GameObjectManager::Instance()->TransformManager()->GetTransform(aabb_object) == nullptr) continue;
			ZMasher::Matrix44f transformA = *GameObjectManager::Instance()->TransformManager()->GetTransform(args->m_Spheres[i]);
			ZMasher::Matrix44f transformB = *GameObjectManager::Instance()->TransformManager()->GetTransform(aabb_object);

			float sqDist = 0.f;
			if (SphereVsAABBTest(args->rad[i], aabb.r, transformA.GetTranslation(), transformB.GetTranslation(), sqDist))
			{
				ZMasher::Matrix44f* transformA = GameObjectManager::Instance()->TransformManager()->GetTransform(args->m_Spheres[i]);
				ZMasher::Matrix44f* transformB = GameObjectManager::Instance()->TransformManager()->GetTransform(aabb_object);
				// Sphere and box are intersecting, separate them.
				ZombieCollisionFeedback(transformA, transformB, sqrt(sqDist) - args->rad[i]);
			}
		}
	}
}