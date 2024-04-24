#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL.h>
#include <iostream>
#include <string.h>
#include <tuple>

enum class MOUSE_BUTTON
{
	left = 0,
	right,
	middle
};

class InputManager
{
public:
	static InputManager* s_Instance;
public:
	static InputManager* GetInstance();

	bool KeyDown(SDL_Scancode scancode);
	bool KeyPressed(SDL_Scancode scancode);
	bool KeyReleased(SDL_Scancode scancode);

	std::tuple<int, int> MousePos();

	bool MouseButtonDown(MOUSE_BUTTON button);
	bool MouseButtonPressed(MOUSE_BUTTON button);
	bool MouseButtonReleased(MOUSE_BUTTON button);

	void Update();
	void UpdatePreviousState();

	inline const bool Quit() const& { return quitFlag; }
private:
	const Uint8* m_KeyboardState;
	Uint8* m_PrevKeyboardState;
	int m_KeyLength;

	Uint32 m_PrevMouseState;
	Uint32 m_MouseState;

	int m_MouseX;
	int m_MouseY;

	bool quitFlag = false;

private:
	InputManager();
	~InputManager();
};

#endif