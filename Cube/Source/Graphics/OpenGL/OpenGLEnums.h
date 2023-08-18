#pragma once

/* Can be static_cast to numbers corresponding to their respective GL enums */
enum class GLMappedBufferAccess : int
{
	/* Same as GL_READ_ONLY */
	ReadOnly = 0x88B8,
	/* Same as GL_WRITE_ONLY */
	WriteOnly = 0x88B9,
	/* Same as GL_READ_WRITE */
	ReadWrite = 0x88BA
};