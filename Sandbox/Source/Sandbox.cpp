#include "Nifty.h"

#include "EngineLayer.h"

class Sandbox : public Application
{
private:

public:
	Sandbox() : Application()
	{
		PushLayer(new EngineLayer());
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