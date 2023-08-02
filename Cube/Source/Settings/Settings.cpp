#include "Settings.h"

Settings::Settings()
{
	renderDistance = 20;
	threadPoolSize = gk::ThreadPool::SystemThreadCount() - 1;
}
