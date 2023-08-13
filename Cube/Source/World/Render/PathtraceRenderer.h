#pragma once

#include "../../Engine/EngineCore.h"

class VertexBufferObject;
class IndexBufferObject;
class VertexArrayObject;
class RasterShader;
class ComputeShader;

class PathtraceRenderer
{
public:

	/* Must be called on the render thread. Will assert this. */
	PathtraceRenderer();
	/* Must be deleted from the render thread. Will assert this. */
	~PathtraceRenderer();

private:

	ComputeShader* pathtraceComputeShader;
	RasterShader* screenShader;
	VertexBufferObject* screenVbo;
	IndexBufferObject* screenIbo;
	VertexArrayObject* screenVao;
};
