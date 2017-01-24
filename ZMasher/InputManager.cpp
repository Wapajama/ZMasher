#include "InputManager.h"

InputManager::InputManager(HINSTANCE instance, HWND hwnd, const int window_x, const int window_y)
	:m_InstanceHandle(instance),
	m_WindowHandle(hwnd)
{
	m_WindowSize.x = window_x;
	m_WindowSize.y = window_y;

	ResetInputStates();

	DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_DirectInput), NULL);

	m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_DirectInputKeyboard, NULL);
	m_DirectInput->CreateDevice(GUID_SysMouse, &m_DirectInputMouse, NULL);

	m_DirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_DirectInputMouse->SetDataFormat(&c_dfDIMouse);
}

void InputManager::ResetInputStates()
{
	for (short i = 0; i < 256; i++)
	{
		m_KeyPressedFlags[i] = false;
		m_KeyReleasedFlags[i] = false;
	}
}

InputManager::~InputManager()
{
}

void InputManager::Update(const float dt)
{
	ResetInputStates();
	m_DirectInputKeyboard->Acquire();
	m_DirectInputKeyboard->GetDeviceState(256, static_cast<LPVOID>(m_Keystate));

	m_DirectInputMouse->Acquire();
	m_DirectInputMouse->GetDeviceState(sizeof(DIMOUSESTATE), static_cast<LPVOID>(&m_Mousestate));


}

bool InputManager::IsKeyDown(unsigned char key)
{
	return m_Keystate[key] & 0x80;
}

bool InputManager::IsKeyUp(unsigned char key)
{
	return !IsKeyDown(key);
}

bool InputManager::IsMouseDown(unsigned char key)
{
	return false;
}

bool InputManager::IsMouseUp(unsigned char key)
{
	return false;
}

LONG InputManager::MouseScrollValue()
{
	return 0;
}

bool InputManager::Create(HINSTANCE instance, HWND hwnd, const int window_x, const int window_y)
{
	m_Instance = new InputManager(instance, hwnd, window_x, window_y);
	return m_Instance != nullptr;
}