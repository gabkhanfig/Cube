#include "Object.h"

darray<IObject*> IObject::objectsPendingDelete[2];
uint8 IObject::thisTickDeleteIndex;
uint8 IObject::nextTickDeleteIndex;

void IObject::_InitializePendingDeleteArray()
{
	thisTickDeleteIndex = 1;
	nextTickDeleteIndex = 0;
	objectsPendingDelete[nextTickDeleteIndex].Reserve(16);
}

void IObject::DeleteAllPendingKillObjects()
{
	for (IObject* object : objectsPendingDelete[thisTickDeleteIndex]) {
		delete object;
	}
	objectsPendingDelete[thisTickDeleteIndex].Empty();
	objectsPendingDelete[thisTickDeleteIndex].Reserve(16);

	thisTickDeleteIndex = (thisTickDeleteIndex + 1) % 2;
	nextTickDeleteIndex = (nextTickDeleteIndex + 1) % 2;
}

void IObject::Destroy()
{
	SetIsPendingDelete();
	objectsPendingDelete[nextTickDeleteIndex].Add(this);
	OnDestroy();
}

void IObject::OnDestroy()
{
}
