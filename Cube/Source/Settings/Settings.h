#pragma once

#include "../Engine/EngineCore.h"

class Settings {

public:

	Settings();

	forceinline int GetRenderDistance() const { return renderDistance; }
	forceinline int GetThreadPoolSize() const { return threadPoolSize; }

private:

	/* Render distance in chunks. */
	int renderDistance;
	/**/
	int threadPoolSize;

};

extern Settings* GetSettings();