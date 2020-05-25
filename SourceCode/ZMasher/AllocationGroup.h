#pragma once
class AllocationGroup
{
public:
	AllocationGroup();
	~AllocationGroup();

	// returns false when reached last element, will begin anew on index 0
	virtual bool Iterate() = 0;

	virtual void Allocate(const int size, void* args) = 0;

private:

};

