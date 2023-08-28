#pragma once

#include "../../Core/EngineTypes.h"

struct DrawArraysIndirectCommand
{
  uint32 vertexCount;
  uint32 instanceCount;
  uint32 first;
  uint32 baseInstance;
};

struct DrawElementsIndirectCommand 
{
  /* Number of indices to draw. */
  uint32  count;
  /* Number of instance of this to draw (eg, 1 for 1 chunk) */
  uint32  instanceCount;
  /*  */
  uint32  firstIndex;
  /* Starting from within vbo */
  int  baseVertex;
  /* gl_InstanceID */
  uint32  baseInstance;
};

struct GLBufferStorageBitmask
{
  enum Flags : uint32 {
    // GL_DYNAMIC_STORAGE_BIT
    DynamicStorage = 0x0100,
    // GL_MAP_READ_BIT
    MapRead = 0x0001,
    // GL_MAP_WRITE_BIT
    MapWrite = 0x0002,
    // GL_MAP_PERSISTENT_BIT
    MapPersistent = 0x0040,
    // GL_MAP_COHERENT_BIT
    MapCoherent = 0x0080,
    // GL_CLIENT_STORAGE_BIT
    ClientStorage = 0x0200
  };

  /* See GLBufferStorageBitmask::Flags */
  GLBufferStorageBitmask(const uint32 flags);
  GLBufferStorageBitmask(const GLBufferStorageBitmask& other);

  const uint32 bitmask;
};