#pragma once

#include "../EngineCore.h"

class TickEngine
{

private:

	/* The maximum allowed delta time, to ensure all physics events occur properly */
	static constexpr float MAX_DELTA_TIME = 0.25;

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