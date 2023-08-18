#include "PathtraceRenderer.h"
#include "../../Engine/OpenGL/OpenGLInstance.h"
#include "../../Engine/Engine.h"
#include "../../Core/Utils/CompileTimeFiles.h"
#include "../../Graphics/OpenGL/Shader/ComputeShader.h"
#include "../../Graphics/OpenGL/Shader/RasterShader.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferLayout.h"
#include <glad/glad.h>
#include "../../Engine/Window/Window.h"
#include <GLFW/glfw3.h>

PathtraceRenderer::PathtraceRenderer() //:
	//pathtraceComputeShader(nullptr),
	//screenShader(nullptr),
	//screenVbo(nullptr),
	//screenIbo(nullptr),
	//screenVao(nullptr)
{
	assertOnRenderThread();

	float vertices[] =
	{
		/*		Positions    |    Texture UVs */
		-1.0f, -1.0f , 0.0f,		0.0f, 0.0f,
		-1.0f,  1.0f , 0.0f,		0.0f, 1.0f,
		 1.0f,  1.0f , 0.0f,		1.0f, 1.0f,
		 1.0f, -1.0f , 0.0f,		1.0f, 0.0f,
	};

	uint32 indices[] =
	{
		0, 2, 1,
		0, 3, 2
	};

	pathtraceComputeShader = new ComputeShader(CompileTimeFiles::GetTextFile("BlockPathtrace.comp")->contents);
	screenShader = new RasterShader(CompileTimeFiles::GetTextFile("BlockPathtrace.vert")->contents, CompileTimeFiles::GetTextFile("BlockPathtrace.frag")->contents);

	screenVbo = VertexBufferObject::Create<float>(vertices, 20);
	//screenIbo = new IndexBufferObject(indices, 6);
	screenIbo = new IndexBufferObject();
	screenIbo->BufferData(indices, 6);

	VertexBufferLayout screenBufferLayout;
	screenBufferLayout.Push<float>(3); // pos
	screenBufferLayout.Push<float>(2); // uvs
	screenVao = new VertexArrayObject();
	//screenVao->Bind();
	screenVao->SetFormatLayout(screenBufferLayout);

	glCreateTextures(GL_TEXTURE_2D, 1, &screenTex);
	glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screenTex, 1, GL_RGBA32F, engine->GetWindow()->GetWidth(), engine->GetWindow()->GetHeight());
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

PathtraceRenderer::~PathtraceRenderer()
{
	assertOnRenderThread();

	delete pathtraceComputeShader;
	delete screenShader;
	delete screenVbo;
	delete screenIbo;
	delete screenVao;
	glDeleteTextures(1, &screenTex);
}

void PathtraceRenderer::PerformTestDraw()
{
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	pathtraceComputeShader->Dispatch(ceil(engine->GetWindow()->GetWidth() / 8), ceil(engine->GetWindow()->GetHeight() / 8), 1);

	screenShader->Bind();
	glBindTextureUnit(0, screenTex);
	screenShader->SetUniform1i("screen", 0);
	screenVao->Bind();
	screenVao->BindVertexBufferObject(screenVbo, 20);
	screenIbo->Bind();
	screenVbo->Bind();
	glDrawElements(GL_TRIANGLES, screenIbo->GetIndexCount(), GL_UNSIGNED_INT, 0);

	
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	gk_assertNotNull(engine);
	gk_assertNotNull(engine->GetWindow());
	gk_assertNotNull(engine->GetWindow()->GetGlfwWindow());
	glfwSwapBuffers(engine->GetWindow()->GetGlfwWindow());
}
