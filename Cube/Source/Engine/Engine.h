#pragma once

#include "EngineCore.h"

class Engine;
class UserInput;
class Window;
class TickEngine;

extern Engine* engine;

class Engine
{


public:

	/*  */
	static void Start();

	/**/
	static void Run(_TickCallback tickCallback);

	forceinline UserInput* GetUserInput() const { return input; }

	forceinline Window* GetWindow() const { return window; }

	forceinline TickEngine* GetTick() const { return tick; }

	float GetDeltaTime() const;

private:

	Engine();

private:

	/**/
	UserInput* input;

	/**/
	Window* window;

	/**/
	TickEngine* tick;

	bool useRenderThread;

	/**/
	gk::Thread* renderThread;


};