#pragma once

#include "EngineCore.h"

class Engine;
class UserInput;
class Window;

extern Engine* engine;

class Engine
{
private:

	/**/
	UserInput* input;

	/**/
	Window* window;

private:

	Engine();

public:

	static void Start();

	forceinline UserInput* GetUserInput() const { return input; }

	forceinline Window* GetWindow() const { return window; }


};