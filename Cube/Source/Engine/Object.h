#pragma once

#include "../Core/EngineTypes.h"

class IObject
{
private:

	static darray<IObject*> objectsPendingKill;

public:

	virtual bool IsPendingKill() = 0;
	virtual void SetIsPendingKill() = 0;

	void Destroy();

	virtual void OnDestroy();

	static void DeleteAllPendingKillObjects();
};

forceinline bool IsValid(IObject* object) {
	object != nullptr && !object->IsPendingKill();
}