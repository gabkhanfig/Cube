#pragma once

#include "EngineCore.h"

class Engine;
class UserInput;
class Window;
class TickEngine;
class GameInstance;

extern Engine* engine;


struct EngineInitializationParams 
{
	GameInstance* gameInstance;
	/* By default, is 2 less than the total threads on the system, leaving the remaining for the primary thread and render threads. */
	uint32 threadPoolNum;
	/**/
	glm::ivec2 resolution;

	EngineInitializationParams();
};

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

	forceinline gk::ThreadPool* GetThreadPool() const { return threadPool; }

	float GetDeltaTime() const;

	void SwapGlfwBuffers();

	void WaitForRenderThread(int64 millisecondTimeout);

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

	/**/
	gk::ThreadPool* threadPool;


};