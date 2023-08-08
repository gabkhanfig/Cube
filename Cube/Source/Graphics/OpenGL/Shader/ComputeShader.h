#pragma once

#include "Shader.h"

class ComputeShader : public Shader
{
public:

	ComputeShader(const char* computeSource);

	void Dispatch(uint32 numGroupsX, uint32 numGroupsY, uint32 numGroupsZ);

	static glm::ivec3 GetMaxWorkGroupsPerComputeShader();
	static glm::ivec3 GetMaxWorkGroupSizes();
	static int GetMaxInvocationsPerWorkGroup();
};