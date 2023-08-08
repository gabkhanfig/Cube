#include "RasterShader.h"
#include <glad/glad.h>

RasterShader::RasterShader(const char* vertexSource, const char* fragmentSource)
{
	const uint32 vertexShader = LoadShader(ShaderType::Vertex, vertexSource);
	const uint32 fragmentShader = LoadShader(ShaderType::Fragment, fragmentSource);

	// Create a shader program object and get a reference to it.
	shaderProgram = glCreateProgram();
	// Attach vertex shader to the shader program.
	glAttachShader(shaderProgram, vertexShader);
	// Attach fragment shader to the shader program.
	glAttachShader(shaderProgram, fragmentShader);
	// Link all of the shaders together into the shader program.
	glLinkProgram(shaderProgram);

	// Delete the shaders that have already been linked.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	Bind();
}
