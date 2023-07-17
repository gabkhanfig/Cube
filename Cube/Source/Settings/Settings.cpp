#include "Settings.h"

Settings::Settings()
{
	renderDistance = 1;
	threadPoolSize = gk::ThreadPool::SystemThreadCount() - 1;
}
