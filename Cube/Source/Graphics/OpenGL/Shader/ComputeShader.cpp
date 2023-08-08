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

glm::ivec3 ComputeShader::GetMaxWorkGroupsPerComputeShader()
{
	int workGroupCount[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workGroupCount[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workGroupCount[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workGroupCount[2]);
	return glm::ivec3(workGroupCount[0], workGroupCount[1], workGroupCount[2]);
}

glm::ivec3 ComputeShader::GetMaxWorkGroupSizes()
{
	int workGroupSizes[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workGroupSizes[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workGroupSizes[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workGroupSizes[2]);
	return glm::ivec3(workGroupSizes[0], workGroupSizes[1], workGroupSizes[2]);

}

int ComputeShader::GetMaxInvocationsPerWorkGroup()
{
	int invocations;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &invocations);
	return invocations;
}
