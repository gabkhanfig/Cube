#include "OpenGLEnums.h"
#include <glad/glad.h>

static_assert(static_cast<int>(GLMappedBufferAccess::ReadOnly) == GL_READ_ONLY);
static_assert(static_cast<int>(GLMappedBufferAccess::WriteOnly) == GL_WRITE_ONLY);
static_assert(static_cast<int>(GLMappedBufferAccess::ReadWrite) == GL_READ_WRITE);