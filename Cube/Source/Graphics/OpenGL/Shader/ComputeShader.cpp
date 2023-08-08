#include "ComputeShader.h"
#include <glad/glad.h>

ComputeShader::ComputeShader(const char* computeSource)
{
	const uint32 computeShader = Shader::LoadShader(ShaderType::Compute, computeSource);

	// Create a shader program object and get a reference to it.
	shaderProgram = glCreateProgram();
	// Attach compute shader to the shader program.
	glAttachShader(shaderProgram, computeShader);
	// Link all of the shaders together into the shader program.
	glLinkProgram(shaderProgram);

	// Delete the shader that have already been linked.
	glDeleteShader(computeShader);

	Bind();
}

void ComputeShader::Dispatch(uint32 numGroupsX, uint32 numGroupsY, uint32 numGroupsZ)
{
	Bind();
	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}
