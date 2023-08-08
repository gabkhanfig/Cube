#pragma once

#include "Shader.h"

class RasterShader : public Shader
{
public:

	RasterShader(const char* vertexSource, const char* fragmentSource);
};