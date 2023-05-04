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

	forceinline bool IsUsingRenderThread() const { return useRenderThread; }

	forceinline gk::Thread* GetRenderThread() const { return renderThread; }

	float GetDeltaTime() const;

	void SwapGlfwBuffers();

private:

	Engine();

	static void InitializeOpenGL(Window* _window, glm::vec3 clearColor);

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