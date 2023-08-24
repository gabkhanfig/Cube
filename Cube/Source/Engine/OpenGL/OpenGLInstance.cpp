#include "OpenGLInstance.h"
#include <glad/glad.h>
#include "../Engine.h"

static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

OpenGLInstance::OpenGLInstance(gk::Thread* renderThread) : 
	renderThreadId(renderThread != nullptr ? renderThread->StdThreadId() : std::this_thread::get_id()),
	isOpenGLInitialized(false)
{}

bool OpenGLInstance::IsExecutingOnRenderThread() const
{
	return std::this_thread::get_id() == renderThreadId;
}

void OpenGLInstance::InitializeOpenGL(gk::Thread* renderThread, const glm::ivec2 viewportSize, const glm::vec3 clearColor)
{
	if (renderThread == nullptr) {
		LoadAndInitializeOpenGL(viewportSize, clearColor);
	}
	else {
		renderThread->BindFunction(std::bind(OpenGLInstance::LoadAndInitializeOpenGL, viewportSize, clearColor));
		renderThread->Execute();
		while(!renderThread->IsReady());
	}

	isOpenGLInitialized = true;
}

void OpenGLInstance::LoadAndInitializeOpenGL(const glm::ivec2 viewportSize, const glm::vec3 clearColor)
{
	assertOnRenderThread();
	gladLoadGL();
	glViewport(0, 0, viewportSize.x, viewportSize.y);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.f);
	glEnable(GL_DEPTH_TEST | GL_DEBUG_OUTPUT);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDebugMessageCallback(DebugMessageCallback, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}
