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
#include "../../Graphics/OpenGL/Render/Renderer.h"

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

	pathtraceComputeShader = new ComputeShader(CompileTimeFiles::GetTextFile("BlockPathtrace.comp"_str)->contents);
	screenShader = new RasterShader(CompileTimeFiles::GetTextFile("BlockPathtrace.vert"_str)->contents, CompileTimeFiles::GetTextFile("BlockPathtrace.frag"_str)->contents);

	screenVbo = new VertexBufferObject();
	screenVbo->BufferData(vertices, 20);
	screenIbo = new IndexBufferObject();
	screenIbo->BufferData(indices, 6);

	screenVao = new VertexArrayObject();
	VertexBufferLayout screenBufferLayout;
	screenBufferLayout.Push<float>(3); // pos
	screenBufferLayout.Push<float>(2); // uvs
	screenVao->SetFormatLayout(screenBufferLayout);
	screenVao->BindVertexBufferObject(screenVbo, 5 * sizeof(GLfloat));
	screenVao->BindIndexBufferObject(screenIbo);

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
	//Renderer::Clear();
	glFrontFace(GL_CCW);

	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindTextureUnit(0, screenTex);
	pathtraceComputeShader->Dispatch(ceil(engine->GetWindow()->GetWidth() / 8), ceil(engine->GetWindow()->GetHeight() / 8), 1);

	screenShader->Bind();
	glBindTextureUnit(0, screenTex);
	screenShader->SetUniform1i("screen"_str, 0);
	screenVao->Bind();
	//screenVao->BindVertexBufferObject(screenVbo, 20);
	//screenVao->BindIndexBufferObject(screenIbo);
	//screenIbo->Bind();
	//screenVbo->Bind();
	//glDrawElements(GL_TRIANGLES, screenIbo->GetIndexCount(), GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, screenIbo->GetIndexCount(), GL_UNSIGNED_INT, 0);

	engine->SwapGlfwBuffers();
}
