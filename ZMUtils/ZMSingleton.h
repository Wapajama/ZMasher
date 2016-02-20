#pragma once

template<typename SingleTonClass>
class ZMSingleton
{
public:
	static SingleTonClass* Instance();

protected:
	ZMSingleton();
	~ZMSingleton();

	static SingleTonClass* m_Instance;
};

template<typename SingleTonClass>
SingleTonClass* ZMSingleton<SingleTonClass>::m_Instance = nullptr;

template<typename SingleTonClass>
SingleTonClass * ZMSingleton<SingleTonClass>::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new SingleTonClass();
	}
	return m_Instance;
}

template<typename SingleTonClass>
ZMSingleton<SingleTonClass>::ZMSingleton()
{
}

template<typename SingleTonClass>
ZMSingleton<SingleTonClass>::~ZMSingleton()
{
}
