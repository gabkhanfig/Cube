#include "Settings.h"

Settings::Settings()
{
	renderDistance = 5;
	threadPoolSize = gk::systemThreadCount() - 1;
}
