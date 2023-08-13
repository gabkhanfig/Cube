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
	screenIbo = new IndexBufferObject(indices, 6);

	VertexBufferLayout screenBufferLayout;
	screenBufferLayout.Push<float>(3); // pos
	screenBufferLayout.Push<float>(2); // uvs
	screenVao = new VertexArrayObject();
	screenVao->SetFormatLayout(screenBufferLayout);
}

PathtraceRenderer::~PathtraceRenderer()
{
	assertOnRenderThread();

	delete pathtraceComputeShader;
	delete screenShader;
	delete screenVbo;
	delete screenIbo;
	delete screenVao;
}
