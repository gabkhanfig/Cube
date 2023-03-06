#pragma once

#include "../../../Engine/EngineCore.h"

/* Shader Storage Buffer Object. 
https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object */
class ShaderBufferObject
{
private:

	uint32 id;

public:

	ShaderBufferObject(const void* data, uint32 size, uint32 index);

	~ShaderBufferObject();

	template<typename T>
	static ShaderBufferObject* Create(const T* data, uint32 num, uint32 index) {
		return new ShaderBufferObject(data, sizeof(T) * num, index);
	}

	void Bind();

	void Unbind();

	/* Binds and then returns a write only pointer to the map buffer. 
	Call UnmapBuffer(); at some point */
	void* GetMapBuffer();

	static void UnmapBuffer();
};

