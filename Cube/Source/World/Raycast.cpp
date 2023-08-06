#include "Raycast.h"

Block* RaycastHitResult::GetHitBlock() const
{
  if (success == HitSuccess::Block) {
    return (Block*)hitObject;
  }
  else {
    return nullptr;
  }
}

Entity* RaycastHitResult::GetHitEntity() const
{
  if (success == HitSuccess::Entity) {
    return (Entity*)hitObject;
  }
  else {
    return nullptr;
  }
}
