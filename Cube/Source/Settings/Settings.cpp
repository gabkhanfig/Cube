#include "Settings.h"

Settings::Settings()
{
	renderDistance = 10;
	threadPoolSize = gk::ThreadPool::SystemThreadCount() - 1;
}
