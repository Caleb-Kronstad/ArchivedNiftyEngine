#pragma once

namespace Nifty
{
	extern std::vector<std::filesystem::path> GetFilesInDirectory(const std::string& path);

	struct SaveItem
	{
		unsigned int id = 0;
		std::string name = "Name";
		std::string type = "Type";
		bool saved = false;
	};
}