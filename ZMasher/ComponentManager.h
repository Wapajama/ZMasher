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

	virtual void RemoveComponentWithGameObject(GameObject object, bool directly = false)=0;

protected:
	//virtual void RemoveComponentWithGameObjectInternal(GameObject object);
};


// Each componentmanager can only contain one single type of component

COMPONENTMANAGER_TEMPLATE
class ComponentManager
	:public IComponentManager
{
public:
	ComponentManager(const int n_components = 1024);
	virtual ~ComponentManager();

	Component* GetComponent(GameObject game_object);
	void AddComponent(Component component, GameObject object);

	void RemoveComponentWithGameObject(GameObject object, bool directly = false)override;

	virtual bool Init() = 0;
	virtual bool Update() = 0;

protected:
	void RemoveComponentWithGameObjectInternal(GameObject object);

	GrowArray<GameObject> m_DeleteObjects;
	GrowArray<Component> m_Components;
	ZMasher::BinarySearchTree<ComponentIndexPair, ComponentComparer> m_LookupSet;
};


COMPONENTMANAGER_TEMPLATE
COMPONENTMANAGER_DECL::ComponentManager(const int n_components) :	m_DeleteObjects(128),
																	m_Components(n_components)
{
}

COMPONENTMANAGER_TEMPLATE
COMPONENTMANAGER_DECL::~ComponentManager()
{
}

COMPONENTMANAGER_TEMPLATE
void COMPONENTMANAGER_DECL::AddComponent(Component component, GameObject object)
{
	m_Components.Add(component);
	m_LookupSet.Insert({ object, m_Components.Size() - 1 });
}

COMPONENTMANAGER_TEMPLATE
bool COMPONENTMANAGER_DECL::Update()
{
	for (int i = 0; i < m_DeleteObjects.Size(); i++)
	{
		this->RemoveComponentWithGameObjectInternal(m_DeleteObjects[i]);
	}	
	return true;
}

COMPONENTMANAGER_TEMPLATE
Component* COMPONENTMANAGER_DECL::GetComponent(GameObject object)
{
	auto comp = m_LookupSet.Find({ object, -1 });
	if (comp == nullptr)
	{
		return nullptr;
	}
	return &m_Components[comp->value.index];
}

COMPONENTMANAGER_TEMPLATE
void COMPONENTMANAGER_DECL::RemoveComponentWithGameObject(GameObject object, bool directly)
{
	m_DeleteObjects.Add(object);
	// if true, call derived componentmanagerclass?
}

COMPONENTMANAGER_TEMPLATE
void COMPONENTMANAGER_DECL::RemoveComponentWithGameObjectInternal(GameObject object)
{
	auto comp = m_LookupSet.Find({ object, -1 });
	if (comp != nullptr)
	{
		// both components have the same index
		m_Components.RemoveCyclic(comp->value.index);

		if (comp->value.index < m_Components.Size() &&
			m_Components.Size() > 0)
		{
			GameObject new_game_object = m_Components[comp->value.index].m_GameObject;
			auto new_comp = m_LookupSet.Find({ new_game_object, -1 });
			new_comp->value.index = comp->value.index;
		}

		m_LookupSet.Delete({ object,-1 });
	}
}