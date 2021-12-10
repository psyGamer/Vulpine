#include "vppch.h"
#include "FileUtil.h"

#include <fstream>

namespace Vulpine
{
	std::vector<char> FileUtil::ReadFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::binary | std::ios::ate);

		VP_ASSERT(!file.is_open(), "Failed to open file: " + filePath);

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> fileBuffer(fileSize); 

		file.seekg(0);
		file.read(fileBuffer.data(), fileSize);
		file.close();

		return std::move(fileBuffer);
	}
	
}