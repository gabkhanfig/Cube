#pragma once

#include "../EngineCore.h"

#define assertOnRenderThread() if(engine != nullptr){ gk_assertm(engine->GetOpenGLInstance()->IsExecutingOnRenderThread(), "Cannot call OpenGL functions on any thread except for the rendering thread"); }

class OpenGLInstance
{
public:
	/* Passing in nullptr means no render thread. */
	OpenGLInstance(gk::Thread* renderThread);

	bool IsExecutingOnRenderThread() const;

	/* Passing in nullptr means no render thread. */
	void InitializeOpenGL(gk::Thread* renderThread, const glm::ivec2 viewportSize, const glm::vec3 clearColor);

private:

	static void LoadAndInitializeOpenGL(const glm::ivec2 viewportSize, const glm::vec3 clearColor);

private:

	const std::thread::id renderThreadId;
	bool isOpenGLInitialized;
};