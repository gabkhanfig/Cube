#include "Shader.h"
#include <glad/glad.h>
#include <GkTypes/File/FileLoader.h>

static GLenum ShaderTypeToGLShader(Shader::Type shaderType) 
{
	switch (shaderType) {
	case Shader::Type::VertexShader:
		return GL_VERTEX_SHADER;
	case Shader::Type::FragmentShader:
		return GL_FRAGMENT_SHADER;
	}
}

uint32 Shader::LoadShader(Shader::Type shaderType, const char* shaderSource)
{
	const GLenum openGLShaderType = ShaderTypeToGLShader(shaderType);

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
		string shaderType;
		switch (openGLShaderType) {
		case GL_VERTEX_SHADER:
			shaderType = "Vertex";
			break;
		case GL_FRAGMENT_SHADER:
			shaderType = "Fragment";
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

uint32 Shader::CreateShader(const char* vertexSource, const char* fragmentSource)
{
	const uint32 vertexShader = LoadShader(Type::VertexShader, vertexSource);
	const uint32 fragmentShader = LoadShader(Type::FragmentShader, fragmentSource);

	// Create a shader program object and get a reference to it.
	uint32 shaderProgram = glCreateProgram();
	// Attach vertex shader to the shader program.
	glAttachShader(shaderProgram, vertexShader);
	// Attach fragment shader to the shader program.
	glAttachShader(shaderProgram, fragmentShader);
	// Link all of the shaders together into the shader program.
	glLinkProgram(shaderProgram);

	// Delete the shaders that have already been linked.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
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

Shader::Shader(const string& vertexString, const string& fragmentString)
{
	shaderProgram = CreateShader(vertexString.CStr(), fragmentString.CStr());
	Bind();
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

void Shader::Bind()
{
	glUseProgram(shaderProgram);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::SetUniform1f(GlobalString uniform, float value)
{
	glUniform1f(GetUniformLocation(uniform), value);
}

void Shader::SetUniform2f(GlobalString uniform, glm::vec2 value)
{
	glUniform2f(GetUniformLocation(uniform), value.x, value.y);
}

void Shader::SetUniform3f(GlobalString uniform, glm::vec3 value)
{
	glUniform3f(GetUniformLocation(uniform), value.x, value.y, value.z);
}

void Shader::SetUniform4f(GlobalString uniform, glm::vec4 value)
{
	glUniform4f(GetUniformLocation(uniform), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform1i(GlobalString uniform, int value)
{
	glUniform1i(GetUniformLocation(uniform), value);
}

void Shader::SetUniform2i(GlobalString uniform, glm::ivec2 value)
{
	glUniform2i(GetUniformLocation(uniform), value.x, value.y);
}

void Shader::SetUniform3i(GlobalString uniform, glm::ivec3 value)
{
	glUniform3i(GetUniformLocation(uniform), value.x, value.y, value.z);
}

void Shader::SetUniform4i(GlobalString uniform, glm::ivec4 value)
{
	glUniform4i(GetUniformLocation(uniform), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMat4f(GlobalString uniform, const glm::mat4& value)
{
	glUniformMatrix4fv(GetUniformLocation(uniform), 1, GL_FALSE, &value[0][0]);
}
