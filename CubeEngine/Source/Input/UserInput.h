#pragma once

#include "../EngineCore.h"

struct GLFWwindow;

enum class EInputAction
{
	Press,
	Release
};

/* Wrapper for GLFW input mods such as shift and control.
https://www.glfw.org/docs/3.3/group__mods.html#ga6b64ba10ea0227cf6f42efd0a220aba1 */
struct InputMods 
{
private:

	uint8 mask;

public:

	InputMods()
		: mask(0)
	{}

	InputMods(int mods) {
		mask = uint8(mods);
	}

	/* One or more Shift keys were held down. */
	forceinline bool Shift() const { return mask & 0x0001; }

	/* One or more Control keys were held down. */
	bool Control() const { return mask & 0x0002; }

	/* One or more Alt keys were held down. */
	bool Alt() const { return mask & 0x0004; }

	/* One or more Super keys were held down. */
	bool Super() const { return mask & 0x0008; }

	/* Caps Lock key is enabled and the GLFW_LOCK_KEY_MODS input mode is set. */
	bool CapsLock() const { return mask & 0x0010; }

	/* Num Lock key is enabled and the GLFW_LOCK_KEY_MODS input mode is set. */
	bool NumLock() const { return mask & 0x0020; }
};

class UserInput
{
public:

	typedef void (*ButtonInputCallback)(GlobalString, EInputAction, InputMods);
	typedef void (*CursorCallback)(double, double);

private:

	glm::dvec2 previousCursorPos;

	ButtonInputCallback buttonInputCallback;

	CursorCallback cursorCallback;

private:

	static EInputAction ActionToInputAction(int action);

	/**/
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/**/
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	/**/
	static void MouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

public:

	UserInput();

	static void SetCallbacks();

	void ButtonInput(int button, int action, int mods);

	void CursorInput(double xpos, double ypos);

	void SetButtonInputCallback(ButtonInputCallback callback);

	void SetCursorPositionCallback(CursorCallback callback);

};

