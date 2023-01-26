#pragma once

#include "../EngineCore.h"

class TickEngine
{

private:

	_TickCallback callback;

	double previous;

	double current;

	float deltaTime;

	float fpsCounter;

public:
	
	TickEngine(_TickCallback tickCallback);

	void RunEngineLoop();

	forceinline float GetDeltaTime() const { return deltaTime; }

	forceinline float CurrentFps() const { return 1 / deltaTime; }

private:

	void UpdateFps();

};