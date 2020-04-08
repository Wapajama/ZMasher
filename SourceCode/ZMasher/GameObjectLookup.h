//#pragma once
//#include <ZMUtils/DataStructures/BinarySearchTree.h>
//#include <ZMUtils/DataStructures/GrowArray.h>
//#include <GameObject.h>
//
//#define GAMEOBJECTLOOKUP_TEMPLATE template<typename SetStruct>
//#define GAMEOBJECTLOOKUP_DECL GameObjectLookup<SetStruct>
//
//GAMEOBJECTLOOKUP_TEMPLATE
//class GameObjectLookup
//{
//public:
//	GameObjectLookup(int initSize = 1024);
//	~GameObjectLookup();
//
//	struct StructIndexPair
//	{
//		int counter;
//		int index;
//	};
//
//	class StructComparer
//		: public ZMasher::BSTComparator<StructIndexPair>
//	{
//	public:
//		bool LessThan(const StructIndexPair& one,const StructIndexPair& two)const override
//		{
//			return one.counter.m_ID < two.counter.m_ID;
//		}
//		bool GreaterThan(const StructIndexPair& one,const StructIndexPair& two)const override
//		{
//			return one.counter.m_ID > two.counter.m_ID;
//		}
//		bool Equals(const StructIndexPair& one,const StructIndexPair& two)const override
//		{
//			return one.counter == two.counter;
//}
//	};
//
//	void Add(const SetStruct& object);
//	void Remove(const SetStruct& object);
//	SetStruct* Get(GameObject object);
//
//private:
//	ZMasher::BinarySearchTree<StructIndexPair, StructComparer> m_LookupSet;
//	GrowArray<SetStruct> m_Array;
//	int m_Counter;
//};
//
//
//GAMEOBJECTLOOKUP_TEMPLATE
//GAMEOBJECTLOOKUP_DECL::GameObjectLookup(int initSize)
//	:m_Array(initSize)
//	,m_Counter(0)
//{
//}
//
//GAMEOBJECTLOOKUP_TEMPLATE
//GAMEOBJECTLOOKUP_DECL::~GameObjectLookup()
//{
//}
//
//GAMEOBJECTLOOKUP_TEMPLATE
//void GAMEOBJECTLOOKUP_DECL::Add(const SetStruct& object)
//{
//	++m_Counter;
//	m_Array.Add(object);
//	m_LookupSet.Insert({object, m_Array.Size() - 1 });
//}
//
//GAMEOBJECTLOOKUP_TEMPLATE
//void GAMEOBJECTLOOKUP_DECL::Remove(const SetStruct& object)
//{
//	auto comp = m_LookupSet.Find({ object, -1 });
//	if (comp != nullptr)
//	{
//		int index = m_Array[comp->value.index];
//		m_Array.RemoveCyclic(comp->value.index);
//
//		if (comp->value.index < m_Array.Size() &&
//			m_Components.Size() > 0)
//		{
//			//auto new_comp = m_LookupSet.Find({ counter, -1 });
//			//new_comp->value.index = comp->value.index;
//		}
//
//		m_LookupSet.Delete({ object,-1 });
//	}
//}
//
//GAMEOBJECTLOOKUP_TEMPLATE
//SetStruct* GAMEOBJECTLOOKUP_DECL::Get(GameObject object)
//{
//	auto comp = m_LookupSet.Find({ object, -1 });
//	if (comp == nullptr)
//	{
//		return nullptr;
//	}
//	return &m_Array[comp->value.index];
//}