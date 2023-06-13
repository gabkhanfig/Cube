#include "Raycast.h"
#include "Block/Block.h"

Block* RaycastHitResult::GetHitBlock() const
{
  if (success == HitSuccess::block) {
    return (Block*)hitObject;
  }
  else {
    return nullptr;
  }
}

Entity* RaycastHitResult::GetHitEntity() const
{
  if (success == HitSuccess::entity) {
    return (Entity*)hitObject;
  }
  else {
    return nullptr;
  }
}
