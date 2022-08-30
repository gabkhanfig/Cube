#pragma once

#include <CubeCore.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

class Shader 
{
private:

	static Shader* boundShader;

	uint32 rendererId;

	std::unordered_map<std::string, uint32> uniforms;

public:

	Shader();

	/* Load vertex and fragment shader into a singular program. */
	static Shader* LoadShader(const std::string& vertexName, const std::string& fragmentName);

	static uint32 CreateShader(uint32 vertexShader, uint32 fragmentShader);

	static uint32 CompileShader(uint32 type, const std::string& source);
	
	static uint32 ParseShader(uint32 type, const std::string& path);

	void Bind();

	void Unbind();

	inline static Shader* GetBoundShader() { return boundShader; }

	uint32 GetUniformLocation(const std::string& uniformName);

	void SetUniform4float(const std::string& uniformName, const glm::vec4& value);

	void SetUniform3float(const std::string& uniformName, const glm::vec3& value);

	void SetUniform1float(const std::string& uniformName, float value);

	void SetUniform1int(const std::string& uniformName, int value);

	void SetUniformMat4float(const std::string& uniformName, const glm::mat4& value);
};