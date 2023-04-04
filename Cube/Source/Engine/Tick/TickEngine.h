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

	/* Get the delta time, or if it's above the maximum allowed by the engine due to lag, you'll get the maximum instead. */
	forceinline float GetDeltaTime() const { return std::min(deltaTime, MAX_DELTA_TIME); }

	forceinline float CurrentFps() const { return 1.f / deltaTime; }

private:

	void UpdateFps();

};