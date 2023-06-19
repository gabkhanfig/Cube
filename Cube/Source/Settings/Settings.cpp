#include "Settings.h"

Settings::Settings()
{
	renderDistance = 4;
	threadPoolSize = gk::ThreadPool::SystemThreadCount() - 1;
}
