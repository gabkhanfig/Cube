#pragma once

#include "../../Core/EngineTypes.h"

struct DrawArraysIndirectCommand
{
  uint32 count;
  uint32 instanceCount;
  uint32 first;
  uint32 baseInstance;
};

struct DrawElementsIndirectCommand 
{
  uint32  count;
  uint32  instanceCount;
  uint32  firstIndex;
  int  baseVertex;
  uint32  baseInstance;
};