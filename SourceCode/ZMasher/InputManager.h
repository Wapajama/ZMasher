#pragma once
#include <Utility/ZMSingleton.h>
#include <dinput.h>
#include <windows.h>
#include <Math\Vector2.h>
#include <bitset>

#define DIK_LMOUSE 0
#define DIK_RMOUSE 1
#define DIK_MIDDLE_MOUSE 2
#define DIK_MOUSEMACRO 3

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

	inline void ClientInFocus(bool focus){m_WindowHasFocus=focus;};
	inline void CursorInsideClientWindow(bool inside){m_MouseInsideClientWindow=inside;};

	LONG MouseScrollValue();
	ZMasher::Vector2i MousePos();
	static bool Create(HINSTANCE instance, HWND hwnd, const int window_x, const int window_y);

private:

	LPDIRECTINPUT8 m_DirectInput;
	ZMasher::Vector2i m_WindowSize;
	
	void ResetInputStates();

	//Keyboard
	LPDIRECTINPUTDEVICE8 m_DirectInputKeyboard;
	BYTE m_Keystate[256];
	BYTE m_PreviousKeyState[256];
	std::bitset<256> m_KeyPressedFlags;
	std::bitset<256> m_KeyReleasedFlags;
	HINSTANCE m_InstanceHandle;
	HWND m_WindowHandle;

	//Mouse
	LPDIRECTINPUTDEVICE8 m_DirectInputMouse;
	std::bitset<4> m_PressedMouseKeyFlags;
	std::bitset<4> m_ReleasedMouseKeyFlags;
	DIMOUSESTATE m_Mousestate;
	DIMOUSESTATE m_PreviousMouseState;
	ZMasher::Vector2i m_MousePos;

	bool m_WindowHasFocus; // Game should ignore user input when not in focus
	bool m_MouseInsideClientWindow;
};

