#pragma once

// Component Collection Enum Data Type
#define CCEDATA_TYPE __int64

enum class ComponentCollectionType : CCEDATA_TYPE
{
	TransformComponent = 1 << 0,
	CollisionComponent = 1 << 1, 
	MeshComponent = 1 << 2, 
	AIComponent = 1 << 3,
};

#define RETURN_COLLECTION_TYPE(getType, thisType) static_cast<ComponentCollectionType>(static_cast<CCEDATA_TYPE>(getType) | static_cast<CCEDATA_TYPE>(thisType))

class ComponentCollection
{
public:
	ComponentCollection(const int count);
	~ComponentCollection();

	inline virtual ComponentCollectionType GetType()const = 0;

	virtual bool Iterate() = 0;
	
	inline void Reset() { m_CurrentIndex = 0; }
	inline const int CurrentIndex() { return m_CurrentIndex; }

private:
	
	const int m_Count;
	int m_CurrentIndex;
};
