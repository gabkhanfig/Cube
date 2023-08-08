#include "Shader.h"
#include <glad/glad.h>
#include <GkTypes/File/FileLoader.h>

static_assert(static_cast<uint32>(ShaderType::Vertex) == GL_VERTEX_SHADER);
static_assert(static_cast<uint32>(ShaderType::Fragment) == GL_FRAGMENT_SHADER);
static_assert(static_cast<uint32>(ShaderType::Compute) == GL_COMPUTE_SHADER);
static_assert(std::is_same_v<uint32, GLenum>);

uint32 Shader::boundId = 0;

uint32 Shader::LoadShader(ShaderType shaderType, const char* shaderSource)
{
	const GLenum openGLShaderType = static_cast<GLenum>(shaderType);

	// Create shader object of specified type and get a reference to it.
	const uint32 shader = glCreateShader(openGLShaderType);
	// Set the shader's source as a string.
	glShaderSource(shader, 1, &shaderSource, NULL);
	// Compile the shader.
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		String shaderType;
		switch (openGLShaderType) {
		case GL_VERTEX_SHADER:
			shaderType = "Vertex";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "Fragment";
			break;
		case GL_COMPUTE_SHADER:
			shaderType = "Compute";
			break;
		default:
			shaderType = "Unknown";
			break;
		}

		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "Shader Compilation Error: " << shaderType << '\n';
		std::cout << infoLog << '\n';
	}

	return shader;
}

Shader::Shader()
{
	shaderProgram = -1;
}
uint32 Shader::GetUniformLocation(GlobalString uniformName)
{
	auto found = uniforms.find(uniformName);
	if (found != uniforms.end()) {
		return found->second;
	}

	const uint32 location = glGetUniformLocation(shaderProgram, uniformName.ToString().CStr());
	if (location == -1) {
		std::cout << "Invalid uniform name: " << uniformName.ToString() << '\n';
	}
	uniforms.insert({ uniformName, location });
	return location;
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

void Shader::Bind()
{
	if (IsBound()) return;
	glUseProgram(shaderProgram);
	boundId = shaderProgram;
}

void Shader::Unbind()
{
	glUseProgram(0);
}

bool Shader::IsBound() const
{
	return boundId == shaderProgram;
}

void Shader::SetUniform1f(GlobalString uniform, float value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniform1f(GetUniformLocation(uniform), value);
}

void Shader::SetUniform2f(GlobalString uniform, glm::vec2 value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniform2f(GetUniformLocation(uniform), value.x, value.y);
}

void Shader::SetUniform3f(GlobalString uniform, glm::vec3 value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniform3f(GetUniformLocation(uniform), value.x, value.y, value.z);
}

void Shader::SetUniform4f(GlobalString uniform, glm::vec4 value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniform4f(GetUniformLocation(uniform), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform1i(GlobalString uniform, int value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniform1i(GetUniformLocation(uniform), value);
}

void Shader::SetUniform2i(GlobalString uniform, glm::ivec2 value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniform2i(GetUniformLocation(uniform), value.x, value.y);
}

void Shader::SetUniform3i(GlobalString uniform, glm::ivec3 value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniform3i(GetUniformLocation(uniform), value.x, value.y, value.z);
}

void Shader::SetUniform4i(GlobalString uniform, glm::ivec4 value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniform4i(GetUniformLocation(uniform), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMat4f(GlobalString uniform, const glm::mat4& value)
{
	gk_assertm(IsBound(), "Cannot set shader uniform on a shader that is not bound");
	glUniformMatrix4fv(GetUniformLocation(uniform), 1, GL_FALSE, &value[0][0]);
}
