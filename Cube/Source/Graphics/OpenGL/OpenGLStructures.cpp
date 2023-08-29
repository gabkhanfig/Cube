#include "OpenGLStructures.h"
#include <glad/glad.h>

static_assert(GLBufferStorageBitmask::DynamicStorage == GL_DYNAMIC_STORAGE_BIT);
static_assert(GLBufferStorageBitmask::Read == GL_MAP_READ_BIT);
static_assert(GLBufferStorageBitmask::Write == GL_MAP_WRITE_BIT);
static_assert(GLBufferStorageBitmask::Persistent == GL_MAP_PERSISTENT_BIT);
static_assert(GLBufferStorageBitmask::Coherent == GL_MAP_COHERENT_BIT);
static_assert(GLBufferStorageBitmask::ClientStorage == GL_CLIENT_STORAGE_BIT);

GLBufferStorageBitmask::GLBufferStorageBitmask(const uint32 flags)
	: bitmask(flags)
{
#if CUBE_DEVELOPMENT
	if (flags == 0) return;
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

GLBufferStorageBitmask::GLBufferStorageBitmask(const GLBufferStorageBitmask& other)
	: bitmask(other.bitmask)
{}

static_assert(GLBufferMapBitmask::Read == GL_MAP_READ_BIT);
static_assert(GLBufferMapBitmask::Write == GL_MAP_WRITE_BIT);
static_assert(GLBufferMapBitmask::Persistent == GL_MAP_PERSISTENT_BIT);
static_assert(GLBufferMapBitmask::Coherent == GL_MAP_COHERENT_BIT);
static_assert(GLBufferMapBitmask::InvalidateRange == GL_MAP_INVALIDATE_RANGE_BIT);
static_assert(GLBufferMapBitmask::InvalidateBuffer == GL_MAP_INVALIDATE_BUFFER_BIT);
static_assert(GLBufferMapBitmask::FlushExplicit == GL_MAP_FLUSH_EXPLICIT_BIT);
static_assert(GLBufferMapBitmask::Unsynchronized == GL_MAP_UNSYNCHRONIZED_BIT);

GLBufferMapBitmask::GLBufferMapBitmask(const uint32 flags)
	: bitmask(flags)
{
#if CUBE_DEVELOPMENT
	if (flags == 0) return;
	/* Perform validation according to OpenGL spec
	https://registry.khronos.org/OpenGL-Refpages/gl4/html/glMapBufferRange.xhtml */
	const bool isRead = flags & GL_MAP_READ_BIT;
	const bool isWrite = flags & GL_MAP_WRITE_BIT;
	gk_assertm(isRead || isWrite, "The map bitmask must be set to read and/or write");

	if (isRead) {
		gk_assertm(!(flags & GL_MAP_INVALIDATE_RANGE_BIT), "Invalidate range flag is incompatible with read flag");
		gk_assertm(!(flags & GL_MAP_INVALIDATE_BUFFER_BIT), "Invalidate buffer flag is incompatible with read flag");
		gk_assertm(!(flags & GL_MAP_UNSYNCHRONIZED_BIT), "Unsynchronize flag is incompatible with read flag");
	}

	if (flags & GL_MAP_FLUSH_EXPLICIT_BIT) {
		gk_assertm(flags & GL_MAP_WRITE_BIT, "If map flush flag is set, write flag must also be set");
	}
#endif
}

GLBufferMapBitmask::GLBufferMapBitmask(const GLBufferMapBitmask& other)
	: bitmask(other.bitmask)
{}
