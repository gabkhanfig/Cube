#include "Object.h"

darray<IObject*> IObject::objectsPendingKill;

void IObject::Destroy()
{
	SetIsPendingKill();
	objectsPendingKill.Add(this);
	OnDestroy();
}

void IObject::OnDestroy()
{
}

void IObject::DeleteAllPendingKillObjects()
{
	for (IObject* object : objectsPendingKill) {
		delete object;
	}
	objectsPendingKill.Empty();
}
