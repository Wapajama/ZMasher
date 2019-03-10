#include "InputManager.h"

InputManager::InputManager(HINSTANCE instance, HWND hwnd, const int window_x, const int window_y)
	:m_InstanceHandle(instance),
	m_WindowHandle(hwnd),
	m_WindowHasFocus(true),
	m_MouseInsideClientWindow(true)
{
	m_WindowSize.x = window_x;
	m_WindowSize.y = window_y;

	ResetInputStates();

	DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_DirectInput), NULL);

	m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_DirectInputKeyboard, NULL);
	m_DirectInput->CreateDevice(GUID_SysMouse, &m_DirectInputMouse, NULL);

	m_DirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_DirectInputMouse->SetDataFormat(&c_dfDIMouse);

	m_DirectInputMouse->SetCooperativeLevel(m_WindowHandle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	m_DirectInputMouse->Acquire();
	for (int i = 0; i < 256; i++)
	{
		m_Keystate[i] = 0;
		m_PreviousKeyState[i] = 0;
		m_KeyPressedFlags[i] = false;
	}
	for (int i = 0; i < 4; i++)
	{
		m_PreviousMouseState.rgbButtons[i] = 0;
		m_Mousestate.rgbButtons[i] = 0;
		m_PressedMouseKeyFlags[i] = false;
	}

}

void InputManager::ResetInputStates()
{
	if (m_WindowHasFocus)
	{
		for (short i = 0; i < 256; i++)
		{
			m_KeyPressedFlags[i] = false;
			m_KeyReleasedFlags[i] = false;
		}
	}
	if (m_MouseInsideClientWindow)
	{
		for (int i = 0; i < 4; i++)
		{
			m_PressedMouseKeyFlags[i] = false;
			m_ReleasedMouseKeyFlags[i] = false;
		}
	}
}

InputManager::~InputManager()
{
}

void InputManager::Update(const float dt)
{
	ResetInputStates();

	HRESULT result;
	//KEYBOARD
	if (m_WindowHasFocus)
	{
		result = m_DirectInputKeyboard->Acquire();
		m_DirectInputKeyboard->GetDeviceState(256, static_cast<LPVOID>(m_Keystate));
	}

	if (m_MouseInsideClientWindow)
	{
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		RECT windowPos;
		GetWindowRect(m_WindowHandle, &windowPos);
		cursorPos.x -= windowPos.left;
		cursorPos.y -= windowPos.top;

		m_MousePos.x = static_cast<int>(cursorPos.x);
		m_MousePos.y = static_cast<int>(cursorPos.y);

		//MOUSE
		result = m_DirectInputMouse->Acquire();
		result = m_DirectInputMouse->GetDeviceState(sizeof(DIMOUSESTATE), static_cast<LPVOID>(&m_Mousestate));

		for (int i = 0; i < 4; i++)
		{
			if (!(m_Mousestate.rgbButtons[i] & 0x80) && (m_PreviousMouseState.rgbButtons[i]))
			{
				m_ReleasedMouseKeyFlags[i] = true;
			}
			if ((m_Mousestate.rgbButtons[i] & 0x80) && !(m_PreviousMouseState.rgbButtons[i]))
			{
				m_PressedMouseKeyFlags[i] = true;
			}
		}
	}
}

bool InputManager::IsKeyDown(unsigned char key)
{
	if (!m_WindowHasFocus)
	{
		return false;
	}
	return m_Keystate[key] & 0x80;
}

bool InputManager::IsKeyUp(unsigned char key)
{
	if (!m_WindowHasFocus)
	{
		return false;
	}
	return !IsKeyDown(key);
}

bool InputManager::IsMouseUp(unsigned char key)
{
	if (!m_WindowHasFocus)
	{
		return false;
	}
	if (!(m_Mousestate.rgbButtons[key] & 0x80))
	{
		return false;
	}
	return true;
}

bool InputManager::IsMouseDown(unsigned char key)
{
	if (!m_WindowHasFocus)
	{
		return false;
	}
	if ((m_Mousestate.rgbButtons[key] & 0x80))
	{
		return true;
	}
	return false;
}

LONG InputManager::MouseScrollValue()
{
	return 0; // NOT IMPLEMENTED
}

ZMasher::Vector2i InputManager::MousePos()
{
	if (m_MouseInsideClientWindow)
	{
		POINT mouse_point;
		GetCursorPos(&mouse_point);
		m_MousePos.x = mouse_point.x;
		m_MousePos.y = mouse_point.y;
	}
	return m_MousePos;
}

bool InputManager::Create(HINSTANCE instance, HWND hwnd, const int window_x, const int window_y)
{
	m_Instance = new InputManager(instance, hwnd, window_x, window_y);
	return m_Instance != nullptr;
}