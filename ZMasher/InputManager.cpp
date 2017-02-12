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

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	RECT windowPos;
	GetWindowRect(m_WindowHandle, &windowPos);
	cursorPos.x -= windowPos.left;
	cursorPos.y -= windowPos.top;

	m_MousePos.x = static_cast<int>(cursorPos.x);
	m_MousePos.y = static_cast<int>(cursorPos.y);
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

ZMasher::Vector2i InputManager::MousePos()
{
	POINT mouse_point;
	GetCursorPos(&mouse_point);
	m_MousePos.x = mouse_point.x;
	m_MousePos.y = mouse_point.y;
	return m_MousePos;
}

bool InputManager::Create(HINSTANCE instance, HWND hwnd, const int window_x, const int window_y)
{
	m_Instance = new InputManager(instance, hwnd, window_x, window_y);
	return m_Instance != nullptr;
}