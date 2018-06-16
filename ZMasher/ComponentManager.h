#pragma once
#include "GameObject.h"
#include <DataStructures\GrowArray.h>
class ComponentManager
{
public:
	ComponentManager();
	virtual ~ComponentManager();

	virtual bool Init()=0;
	virtual void Destroy(){};
	virtual bool Update() = 0;

protected:

	template <typename ComponentList>
	void RemoveDeadComponents(ComponentList& component_list);
	inline int PopFreeIndex() 
	{
		if (!m_FreeIndexes.Empty())
		{
			const int free_index = m_FreeIndexes.GetLast();
			m_FreeIndexes.RemoveLast();
			return free_index;
		}
		return m_FreeIndexes.found_none;
	}
	// Will be called after each removed component in RemoveDeadComponents
	virtual void PostRemoveDeadComponents(GameObject) {}
	GrowArray<int> m_FreeIndexes;
};

template <typename ComponentList>
void ComponentManager::RemoveDeadComponents(ComponentList& component_list)
{
	for (short i = component_list.Size() - 1; i >= 0; --i)
	{
		GameObject& go = component_list[i].m_GameObject;
		if (go != NULL_GAME_OBJECT &&
			GAME_OBJECT_IS_ALIVE(go) == false)
		{
			PostRemoveDeadComponents(go);
			go = NULL_GAME_OBJECT;
			m_FreeIndexes.Add(i);
		}
	}
}