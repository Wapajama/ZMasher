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


#define COMPONENTGROUP_TEMPLATE template<typename Component>
#define COMPONENTGROUP_DECL ComponentGroup<Component>

COMPONENTGROUP_TEMPLATE
class ComponentGroup
{
public:
	ComponentGroup(const int startIndex, const int count, GrowArray<Component, int>& components);
	~ComponentGroup();

	inline bool Iterate() { return ++m_CurrentIndex >= m_Count ? false : true; }
	inline void Reset() { m_Current>Index = 0; }
	inline Component& Current() { return m_Components[m_CurrentIndex + m_StartIndex]; }
	inline Component& Get(const int i) { return m_Components[i]; }
	__forceinline const int StartIndex() { return m_StartIndex; }
	__forceinline const int Count() { return m_Count; }

	// for the component manager to know if a component is inside of this group
	inline bool IsIndexInGroup(const int index) { return index >= m_StartIndex && index < m_StartIndex + m_Count;}

private:
	const int m_StartIndex;
	const int m_Count;
	int m_CurrentIndex;
	// TODO: Refactor this to disable the component group from acccessing the entire component list
	// e.g through returning a sub-array of the original array
	GrowArray<Component, int>& m_Components;
};

COMPONENTGROUP_TEMPLATE
COMPONENTGROUP_DECL::ComponentGroup(const int startIndex, const int count, GrowArray<Component, int>& components)
	: m_StartIndex(startIndex)
	, m_Count(count)
	, m_Components(components)
	, m_CurrentIndex(0)
{
}

COMPONENTGROUP_TEMPLATE
COMPONENTGROUP_DECL::~ComponentGroup()
{
}


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

	__forceinline int Count() {return m_Components.Size();}


	// Creates [count] uninitialized components
	ComponentGroup<Component>* CreateComponentGroup(const int count, const GrowArray<GameObject>& game_objects);
	// Creates [count] components initialized with [component]
	ComponentGroup<Component>* CreateComponentGroup(const int count, const Component& component, const GrowArray<GameObject>& game_objects);
	// Creates a ComponentGroup of componets initialized with [components], [repeat] amount of times
	ComponentGroup<Component>* CreateComponentGroup(const GrowArray<Component>& components, const GrowArray<GameObject>& game_objects);

protected:
	void RemoveComponentWithGameObjectInternal(GameObject object);

	inline bool IsInComponentGroup(const int index);
	GrowArray<GameObject> m_DeleteObjects;
	GrowArray<Component, int> m_Components;
	ZMasher::BinarySearchTree<ComponentIndexPair, ComponentComparer> m_LookupSet;

	GrowArray<ComponentGroup<Component>*> m_ComponentGroups;
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
	component.m_GameObject = object;
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
	m_DeleteObjects.RemoveAll();
	return true;
}

COMPONENTMANAGER_TEMPLATE
ComponentGroup<Component>* COMPONENTMANAGER_DECL::CreateComponentGroup(const int count, const GrowArray<GameObject>& game_objects)
{
	return CreateComponentGroup(count, Component(), game_objects);
}

COMPONENTMANAGER_TEMPLATE
ComponentGroup<Component>* COMPONENTMANAGER_DECL::CreateComponentGroup(const int count, const Component& component, const GrowArray<GameObject>& game_objects)
{
	ASSERT(game_objects.Size() == count, "Component count and game object count must match!");
	if (game_objects.Size() != count)
	{
		return nullptr;
	}
	// TODO: fix better memory allocation for this...?
	ComponentGroup<Component>* group = new ComponentGroup<Component>(m_Components.Size(), count, m_Components);
	for (int i = 0; i < count; i++)
	{
		this->AddComponent(component, game_objects[i]);
	}
	m_ComponentGroups.Add(group);
	return group;
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
inline bool COMPONENTMANAGER_DECL::IsInComponentGroup(const int index)
{
	for (int i = 0; i < m_ComponentGroups.Size(); ++i)
	{
		if (m_ComponentGroups[i].IsIndexInGroup(i))
		{
			return true;
		}
	}
	return false
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
		this->DeleteComponentCallback(object);
		m_LookupSet.Delete({ object,-1 });
	}
}

