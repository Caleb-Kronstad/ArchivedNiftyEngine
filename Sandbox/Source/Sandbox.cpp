#include "Nifty.h"

#include "GameLayer.h"

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