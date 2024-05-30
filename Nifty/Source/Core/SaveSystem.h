#pragma once

#include "nfpch.h"

namespace Nifty
{
	class SaveSystem
	{
	public:

	};

	class SaveItem
	{
	public:
		unsigned int id = 0;
		std::string name = "Name";
		std::string type = "Type";
		bool saved = false;
	};
}