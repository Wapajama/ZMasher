#pragma once
#include "GameObject.h"
#include <DataStructures\GrowArray.h>
#include <DataStructures\BinarySearchTree.h>

#define COMPONENTMANAGER_TEMPLATE template<typename Component>
#define COMPONENTMANAGER_DECL ComponentManager<Component>

struct ComponentIndexPair
{
	GameObject game_object;
	int index;//index in the array in TransformComponentManager
};

class ComponentComparer
	: public ZMasher::BSTComparator<ComponentIndexPair>
{
public:
	bool LessThan(const ComponentIndexPair& one, const ComponentIndexPair& two)const override
	{
		return one.game_object.m_ID < two.game_object.m_ID;
	}
	bool GreaterThan(const ComponentIndexPair& one, const ComponentIndexPair& two)const override
	{
		return one.game_object.m_ID > two.game_object.m_ID;
	}
	bool Equals(const ComponentIndexPair& one, const ComponentIndexPair& two)const override
	{
		return one.game_object == two.game_object;
	}

};

class IComponentManager
{
public:
	IComponentManager() {};
	~IComponentManager() {};

	virtual bool Init() = 0;
	virtual void Destroy() {};
	virtual bool Update() = 0;
	virtual void DeleteComponentCallback(GameObject component){};

	virtual void RemoveComponentWithGameObject(GameObject object, bool directly = false)=0;
};
#define MAX_COMPONENTS 1024*64
#define COMPONENTINDEXTYPE_NULL UINT16_MAX // INT32_MAX
typedef unsigned short ComponentIndexType;
COMPONENTMANAGER_TEMPLATE
class ComponentManager
	:public IComponentManager
{
public:
	ComponentManager();
	virtual ~ComponentManager();

	Component* GetComponent(GameObject game_object);
	void AddComponent(Component component, GameObject object);

	virtual void RemoveComponentWithGameObject(GameObject object, bool directly = false)override;

	virtual bool Init() = 0;
	virtual bool Update() = 0;

	__forceinline int Count() {return m_Components.Size();}

protected:
	void RemoveComponentWithGameObjectInternal(GameObject object);

	struct ComponentIndex
	{
		ComponentIndexType index;
	};

	GrowArray<ComponentIndex, int, MAX_GAME_OBJS> m_Indices;
	GrowArray<Component, int, MAX_COMPONENTS> m_Components;
	GrowArray<GameObject, int, MAX_GAME_OBJS> m_DeleteObjs;
	// ZMasher::BinarySearchTree<ComponentIndexPair, ComponentComparer> m_LookupSet;

	//class ComponentGroup
	//{
	//public:
	//	ComponentGroup();
	//	~ComponentGroup();


	//};
	//GrowArray<ComponentGroup> m_ComponentGroups;
};

COMPONENTMANAGER_TEMPLATE
COMPONENTMANAGER_DECL::ComponentManager()
{
	//m_FreeList = COMPONENTINDEXTYPE_NULL;
	for (int i = 0; i < MAX_GAME_OBJS; i++)
	{
		ComponentIndex index;
		index.index = COMPONENTINDEXTYPE_NULL;
		m_Indices.Add(index);
	}
}

COMPONENTMANAGER_TEMPLATE
COMPONENTMANAGER_DECL::~ComponentManager()
{
}

COMPONENTMANAGER_TEMPLATE
void COMPONENTMANAGER_DECL::AddComponent(Component component, GameObject object)
{
	//if (m_FreeList == COMPONENTINDEXTYPE_NULL)
	//{
	GO_ID_TYPE id = m_Indices[object.Index()].index;
	m_Indices[object.Index()].index = m_Components.Size();
	m_Components.Add(component);
	//}
	//else
	//{
	//	m_Indices[object.Index()].index = m_FreeList;
	//	m_FreeList = m_Indices[object.Index()].next;
	//	m_Components[m_FreeList] = component;
	//}
	// m_LookupSet.Insert({ object, m_Components.Size() - 1 });
}

COMPONENTMANAGER_TEMPLATE
bool COMPONENTMANAGER_DECL::Update()
{
	//for (int i = 0; i < m_DeleteObjects.Size(); i++)
	//{
	//	this->RemoveComponentWithGameObjectInternal(m_DeleteObjects[i]);
	//}

	for (int i = 0; i < m_DeleteObjs.Size(); i++)
	{
		this->RemoveComponentWithGameObject(m_DeleteObjs[i], true);
	}
	m_DeleteObjs.RemoveAll();
	return true;
}

COMPONENTMANAGER_TEMPLATE
Component* COMPONENTMANAGER_DECL::GetComponent(GameObject object)
{
	//auto comp = m_LookupSet.Find({ object, -1 });
	//if (comp == nullptr)
	//{
	//	return nullptr;
	//}
	//return &m_Components[comp->value.index];
	if (m_Indices[object.Index()].index == COMPONENTINDEXTYPE_NULL)
	{
		return nullptr;
	}
	return &m_Components[m_Indices[object.Index()].index];

}

COMPONENTMANAGER_TEMPLATE
void COMPONENTMANAGER_DECL::RemoveComponentWithGameObject(GameObject object, bool directly)
{
	if (m_Indices[object.Index()].index == COMPONENTINDEXTYPE_NULL)
	{
		return;
	}

	if (directly)
	{
		// 1. Get the game object of the component lying last
		GameObject go = m_Components.GetLast().m_GameObject;
		// 2. last component is put in the removed one's slot
		GO_ID_TYPE id = m_Indices[object.Index()].index;
		m_Components.RemoveCyclic(m_Indices[object.Index()].index);
		// 3. the previous game object's ID is now dangling, update it to the new position
		m_Indices[go.Index()].index = m_Indices[object.Index()].index;
		// 4. nullify the old one to indicate that it's removed
		m_Indices[object.Index()].index = COMPONENTINDEXTYPE_NULL;
	}
	else
	{
		m_DeleteObjs.Add(object);
	}



	//GameObject temp = m_Components.GetLast().m_GameObject;
	//m_Components.RemoveCyclic(m_Indices[object.Index()].index);
	//m_Indices[object.Index()].next = m_FreeList;
	//m_FreeList = m_Indices[object.Index()].index;
	//m_Indices[object.Index()].index = COMPONENTINDEXTYPE_NULL;
	//m_DeleteObjects.Add(object);
	// if true, call derived componentmanagerclass?
}

COMPONENTMANAGER_TEMPLATE
void COMPONENTMANAGER_DECL::RemoveComponentWithGameObjectInternal(GameObject object)
{
	//auto comp = m_LookupSet.Find({ object, -1 });
	//if (comp != nullptr)
	//{
	//	// both components have the same index
	//	m_Components.RemoveCyclic(comp->value.index);

	//	if (comp->value.index < m_Components.Size() &&
	//		m_Components.Size() > 0)
	//	{
	//		GameObject new_game_object = m_Components[comp->value.index].m_GameObject;
	//		auto new_comp = m_LookupSet.Find({ new_game_object, -1 });
	//		new_comp->value.index = comp->value.index;
	//	}
	//	this->DeleteComponentCallback(object);
	//	m_LookupSet.Delete({ object,-1 });
	//}
}

