#pragma once

extern void CrashProgram(const char* msg = nullptr);

template<typename SingleTonClass>
class ZMSingleton
{
public:
	static SingleTonClass* Instance();

	static void Release();
	static void Create();

protected:
	ZMSingleton();
	virtual ~ZMSingleton();

	virtual bool Init() { return true; };

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
	delete m_Instance;
}

template<typename SingleTonClass>
void ZMSingleton<SingleTonClass>::Create()
{
	m_Instance = new SingleTonClass();
	if (m_Instance->Init() == false)
	{
		CrashProgram("Singleton failed to create!");
	}
}

template<typename SingleTonClass>
ZMSingleton<SingleTonClass>::ZMSingleton()
{
}

template<typename SingleTonClass>
ZMSingleton<SingleTonClass>::~ZMSingleton()
{
}
