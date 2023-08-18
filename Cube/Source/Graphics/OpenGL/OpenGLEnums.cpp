#include "OpenGLEnums.h"
#include <glad/glad.h>

static_assert(static_cast<int>(GLBufferAccess::ReadOnly) == GL_READ_ONLY);
static_assert(static_cast<int>(GLBufferAccess::WriteOnly) == GL_WRITE_ONLY);
static_assert(static_cast<int>(GLBufferAccess::ReadWrite) == GL_READ_WRITE);