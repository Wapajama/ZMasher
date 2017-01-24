#pragma once
#include <Utility/ZMSingleton.h>
#include <dinput.h>
#include <windows.h>
#include <Math\Vector2.h>
#include <bitset>

class InputManager:
	public ZMSingleton<InputManager>
{
	InputManager(const HINSTANCE instance, const HWND hwnd, const int window_x, const int window_y);
	~InputManager();
public:

	void Update(const float dt);

	bool IsKeyDown(unsigned char key);
	bool IsKeyUp(unsigned char key);

	bool IsMouseDown(unsigned char key);
	bool IsMouseUp(unsigned char key);

	LONG MouseScrollValue();

	static bool Create(HINSTANCE instance, HWND hwnd, const int window_x, const int window_y);

private:

	LPDIRECTINPUT8 m_DirectInput;
	LPDIRECTINPUTDEVICE8 m_DirectInputKeyboard;
	LPDIRECTINPUTDEVICE8 m_DirectInputMouse;

	DIMOUSESTATE m_Mousestate;
	DIMOUSESTATE m_PreviousMouseState;

	void ResetInputStates();

	ZMasher::Vector2f m_MousePos;
	ZMasher::Vector2i m_WindowSize;

	BYTE m_Keystate[256];
	BYTE m_PreviousKeyState[256];

	std::bitset<256> m_KeyPressedFlags;
	std::bitset<256> m_KeyReleasedFlags;
	HINSTANCE m_InstanceHandle;
	HWND m_WindowHandle;
};

