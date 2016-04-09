#pragma once

template<typename SingleTonClass>
class ZMSingleton
{
public:
	static SingleTonClass* Instance();

	static void Release();
	static void Create();

protected:
	ZMSingleton();
	~ZMSingleton();

	virtual void Init() {};

	static SingleTonClass* m_Instance;
};

template<typename SingleTonClass>
SingleTonClass* ZMSingleton<SingleTonClass>::m_Instance = nullptr;

template<typename SingleTonClass>
SingleTonClass * ZMSingleton<SingleTonClass>::Instance()
{
	return m_Instance;
}

template<typename SingleTonClass>
void ZMSingleton<SingleTonClass>::Release()
{
	delete m_Instance:
}

template<typename SingleTonClass>
void ZMSingleton<SingleTonClass>::Create()
{
	m_Instance = new SingleTonClass();
}

template<typename SingleTonClass>
ZMSingleton<SingleTonClass>::ZMSingleton()
{
}

template<typename SingleTonClass>
ZMSingleton<SingleTonClass>::~ZMSingleton()
{
}
