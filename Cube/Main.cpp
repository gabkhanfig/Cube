#include <glad/glad.h>
#include "Source/Engine/Engine.h"
#include <GLFW/glfw3.h>

#include "Source/Player/Player.h"
#include "Source/GameInstance.h"
#include "Source/Settings/Settings.h"

#include "Testing/CubeTest.h"
#include "Source/Graphics/OpenGL/Shader/RasterShader.h"
#include "Source/Graphics/OpenGL/Shader/ComputeShader.h"
#include "Source/Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "Source/Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "Source/Graphics/OpenGL/Buffers/IndexBufferObject.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Source/Core/Utils/CompileTimeFiles.h"

GameInstance* gameInstance;
Settings* settings;

GameInstance* GetGameInstance() {
  return gameInstance;
}

Settings* GetSettings() {
  return settings;
}

//int main(int argc, char** argv)
//{
//#if RUN_TESTS
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//#else
//  settings = new Settings();
//  Engine::Initialize();
//  gameInstance = new GameInstance();
//  gameInstance->Init();
//	gk::Event<void, float>* gameInstanceTickEvent = gk::Event<void, float>::Create(gameInstance, &GameInstance::Tick);
//  Engine::Run(gameInstanceTickEvent);
//  return 0;
//#endif
//}

const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT = 1080;

const unsigned short OPENGL_MAJOR_VERSION = 4;
const unsigned short OPENGL_MINOR_VERSION = 6;

bool vSync = true;



GLfloat vertices[] =
{
	-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};


int main()
{
	CompileTimeFiles::LoadAllFiles();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Compute Shaders", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create the GLFW window\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(vSync);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	VertexBufferObject* vbo = new VertexBufferObject();
	vbo->BufferData(vertices, 20);
	IndexBufferObject* ibo = new IndexBufferObject();
	ibo->BufferData(indices, 6);

	VertexArrayObject* vao = new VertexArrayObject();
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(2);

	vao->SetFormatLayout(vbl);
	vao->BindVertexBufferObject(vbo, 5 * sizeof(GLfloat));
	vao->BindIndexBufferObject(ibo);

	GLuint screenTex;
	glCreateTextures(GL_TEXTURE_2D, 1, &screenTex);
	glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screenTex, 1, GL_RGBA32F, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	RasterShader* screenShader = new RasterShader(CompileTimeFiles::GetTextFile("BlockPathtrace.vert")->contents, CompileTimeFiles::GetTextFile("BlockPathtrace.frag")->contents);
	ComputeShader* computeShader = new ComputeShader(CompileTimeFiles::GetTextFile("BlockPathtrace.comp")->contents);

	while (!glfwWindowShouldClose(window))
	{
		computeShader->Dispatch(ceil(SCREEN_WIDTH / 8), ceil(SCREEN_HEIGHT / 8), 1);

		screenShader->Bind();
		glBindTextureUnit(0, screenTex);
		screenShader->SetUniform1i("screen", 0);
		//glBindVertexArray(VAO);
		vao->Bind();
		//vbo->Bind();
		//ibo->Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
}