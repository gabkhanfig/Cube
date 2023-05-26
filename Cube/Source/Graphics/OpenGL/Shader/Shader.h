#pragma once

#include "../../../Engine/EngineCore.h"

class Shader 
{
public:

	enum class Type : uint8
	{
		VertexShader,
		FragmentShader
	};

private:

	uint32 shaderProgram;

	std::unordered_map<GlobalString, uint32> uniforms;

private:

	static uint32 LoadShader(Shader::Type shaderType, const char* shaderSource);

	static uint32 CreateShader(const char* vertexSource, const char* fragmentSource);

	uint32 GetUniformLocation(GlobalString uniformName);

public:

	/* Construct a shader given strings containing vertex and fragment source code. */
	Shader(const char* vertexString, const char* fragmentString);

	~Shader();

	/* Bind this shader for use until the next clear. */
	void Bind();

	/* Unbind this shader (or whatever shader happens to be bound technically). */
	void Unbind();

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

};