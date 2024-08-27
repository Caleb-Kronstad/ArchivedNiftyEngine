#include "nfpch.h"
#include "FileUtility.h"

namespace Nifty
{
	std::vector<std::filesystem::path> GetFilesInDirectory(const std::string& path)
	{
		std::vector<std::filesystem::path> files_in_directory;
		for (const auto& file : std::filesystem::directory_iterator(path))
			files_in_directory.push_back(file.path());
		return files_in_directory;
	}
}