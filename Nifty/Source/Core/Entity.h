#pragma once

namespace Nifty
{
	class Entity
	{
	public:
		unsigned int id = 0;
		std::string name = "Name";
		std::string type = "Entity";
		bool saved = false;
	};
}

