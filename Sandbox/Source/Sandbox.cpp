#include "Nifty.h"

class GameLayer : public EngineLayer
{

};

class Sandbox : public Application
{
private:

public:
	Sandbox() : Application()
	{
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{
	}
};

int main(int argc, char** argv)
{
	auto app = new Sandbox();
	app->Run();
	delete app;

	return 0;
}