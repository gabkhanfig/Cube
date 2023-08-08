#pragma once

#include "../../../Engine/EngineCore.h"

enum class ShaderType : uint32
{
	/* Same value as GL_VERTEX_SHADER */
	Vertex = 0x8B31,
	/* Same value as GL_FRAGMENT_SHADER */
	Fragment = 0x8B30,
	/* Same value as GL_COMPUTE_SHADER */
	Compute = 0x91B9
};

/* See RasterShader and ComputeShader subclasses. */
class Shader 
{
public:

	~Shader();

	/* Bind this shader for use until the next clear. */
	void Bind();

	/* Unbind this shader (or whatever shader happens to be bound technically). */
	void Unbind(); 
	
	bool IsBound() const;

	/* Set shader uniform of 1 float. */
	void SetUniform1f(GlobalString uniform, float value);

	/* Set shader uniform of 2 floats. */
	void SetUniform2f(GlobalString uniform, glm::vec2 value);

	/* Set shader uniform of 3 floats. */
	void SetUniform3f(GlobalString uniform, glm::vec3 value);

	/* Set shader uniform of 4 floats. */
	void SetUniform4f(GlobalString uniform, glm::vec4 value);

	/* Set shader uniform of 1 signed integer. */
	void SetUniform1i(GlobalString uniform, int value);

	/* Set shader uniform of 2 signed integers. */
	void SetUniform2i(GlobalString uniform, glm::ivec2 value);

	/* Set shader uniform of 3 signed integers. */
	void SetUniform3i(GlobalString uniform, glm::ivec3 value);

	/* Set shader uniform of 4 signed integers. */
	void SetUniform4i(GlobalString uniform, glm::ivec4 value);

	/* Set shader uniform of a 4N matrix of floats. */
	void SetUniformMat4f(GlobalString uniform, const glm::mat4& value);

protected:

	static uint32 LoadShader(ShaderType shaderType, const char* shaderSource);

	Shader();

	uint32 GetUniformLocation(GlobalString uniformName);

protected:

	uint32 shaderProgram;

	std::unordered_map<GlobalString, uint32> uniforms;

private:

	static uint32 boundId;

};