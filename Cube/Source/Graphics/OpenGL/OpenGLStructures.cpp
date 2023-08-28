#include "OpenGLStructures.h"
#include <glad/glad.h>

static_assert(GLBufferStorageBitmask::DynamicStorage == GL_DYNAMIC_STORAGE_BIT);
static_assert(GLBufferStorageBitmask::MapRead == GL_MAP_READ_BIT);
static_assert(GLBufferStorageBitmask::MapWrite == GL_MAP_WRITE_BIT);
static_assert(GLBufferStorageBitmask::MapPersistent == GL_MAP_PERSISTENT_BIT);
static_assert(GLBufferStorageBitmask::MapCoherent == GL_MAP_COHERENT_BIT);
static_assert(GLBufferStorageBitmask::ClientStorage == GL_CLIENT_STORAGE_BIT);

GLBufferStorageBitmask::GLBufferStorageBitmask(const uint32 flags)
	: bitmask(flags)
{
#if CUBE_DEVELOPMENT
	/* Perform validation according to OpenGL spec
	https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml */
	if (flags & GL_MAP_PERSISTENT_BIT) {
		const bool isReadAndOrWrite = bool(flags & GL_MAP_READ_BIT) || bool(flags & GL_MAP_WRITE_BIT);
		gk_assertm(isReadAndOrWrite, "GLBufferStorageBitmask using persistent mapping must contain read and/or write flags too.\nAdd GLBufferStorageBitmask::MapRead or GLBufferStorageBitmask::MapWrite to the bitmask.");
	}
	if (flags & GL_MAP_COHERENT_BIT) {
		gk_assertm(flags & GL_MAP_PERSISTENT_BIT, "GLBufferStorageBitmask using coherent mapping must also have persistent mapping.\n Add GLBufferStorageBitmask::MapPersistent to the bitmask.");
	}
#endif
}
