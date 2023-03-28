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
  /* Number of vertices */
  uint32  vertexCount;
  /* Number of instance of this to draw (eg, 1 for 1 chunk) */
  uint32  instanceCount;
  /*  */
  uint32  firstIndex;
  /* Starting from within vbo */
  int  baseVertex;
  /* gl_InstanceID */
  uint32  baseInstance;
};