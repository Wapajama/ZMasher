#pragma once
//#include "ZMModel.h"
#include <DirectXMath.h>
#include <ZMVector.h>

class ZMModel;

class ZMModelInstance
{
public:
	ZMModelInstance(void);
	~ZMModelInstance(void);

	inline void SetPosition(const ZMasher::Vector3f& position);
	inline void Rotate(const float degrees);

	inline ZMasher::Vector3f GetPosition();
	inline ZMasher::Vector3f GetRotation();

	__forceinline ZMModel* GetModel();
	__forceinline void SetModel(ZMModel* model);

private:

	ZMModel* m_Model;//replace with a has instead? Is this needed other than just getting the model to render?

	/*
		TODO:	implement own mathlibrary, SIMD optimized? 
				What is/how does SIMD work?
				For now, DirectX math
	*/

	ZMasher::Vector3f m_Position;
	ZMasher::Vector3f m_Rotation;

};

inline void					ZMModelInstance::SetPosition(const ZMasher::Vector3f& position)
{
	m_Position = position;
}
inline void					ZMModelInstance::Rotate(const float degrees)
{
	assert(false);//NOT IMPLEMENTED!!
}
inline ZMasher::Vector3f	ZMModelInstance::GetPosition()
{
	return m_Position;
}
inline ZMasher::Vector3f	ZMModelInstance::GetRotation()
{
	return m_Rotation;
}
__forceinline ZMModel*		ZMModelInstance::GetModel()
{
	return m_Model;
}
__forceinline void			ZMModelInstance::SetModel(ZMModel* model)
{
	m_Model = model;
}