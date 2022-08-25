#pragma once

#include "RenderSettings.h"
#include <CubeCore.h>
#include <glad/glad.h>

class Renderer
{
private:

private:

	

public:

	static void LoadGraphicsAPI();
	 
	static void DrawTriangles(uint32 triangleCount);

	static void DrawQuads(uint32 quadCount);

	static void Clear();

#pragma region OpenGL

	static void OpenGLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

#pragma endregion

	static void Frame(double deltaTime);
};

