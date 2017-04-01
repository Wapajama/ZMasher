#pragma once
#include "GameObject.h"
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

};

template <typename ComponentList>
void ComponentManager::RemoveDeadComponents(ComponentList& component_list)
{
	for (short i = component_list.Size() - 1; i >= 0; --i)
	{
		if (GAME_OBJECT_IS_ALIVE(component_list[i].m_GameObject) == false)
		{
			component_list.RemoveCyclic(i);
		}
	}
}