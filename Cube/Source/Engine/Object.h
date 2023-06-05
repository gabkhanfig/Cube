#pragma once

#include "../Core/EngineTypes.h"
 
/* Interface for all game objects that need lifetime tracking. Upon calling IObject::Destroy, 
the object will be stored in an array to be fully deleted the NEXT tick. This way,
any references to the object can be cleaned up. Use IsValid to check if an object is non-null and not pending delete. */
class IObject
{
public:

	static void _InitializePendingDeleteArray();
	static void DeleteAllPendingKillObjects();

	void Destroy();

	virtual void OnDestroy();

	virtual bool IsPendingDelete() const = 0;

	/* DO NOT CALL MANUALLY. */
	virtual void SetIsPendingDelete() = 0;

private:

	// Two pending delete buffers to allow reference cleanup the following tick.
	static darray<IObject*> objectsPendingDelete[2];
	static uint8 thisTickDeleteIndex;
	static uint8 nextTickDeleteIndex;

};

forceinline bool IsValid(IObject* object) {
	object != nullptr && !object->IsPendingDelete();
}