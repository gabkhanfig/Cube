#include "Settings.h"

Settings::Settings()
{
	renderDistance = 5;
	threadPoolSize = gk::ThreadPool::SystemThreadCount() - 1;
}
