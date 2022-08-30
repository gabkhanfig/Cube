#pragma once

class TickManager
{

private:

	static TickManager* tickManager;

	double previous;

	double current;

	double deltaTime;
	
public:

	inline static double getDeltaTime() { return TickManager::tickManager->deltaTime; }

	static void RunGameLoop();

	void Tick();

	void TestImGuiFrame();

};

