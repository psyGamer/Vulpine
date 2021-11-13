#include "vppch.h"
#include "FileUtil.h"

#include <fstream>

namespace Vulpine
{
	void FileUtil::ReadFile(const std::string& filePath, std::vector<char>& fileBuffer)
	{
		std::ifstream file(filePath, std::ios::binary | std::ios::ate);

		VP_ASSERT(!file.is_open(), "Failed to open file: " + filePath);

		size_t fileSize = (size_t)file.tellg();
		fileBuffer.resize(fileSize);

		file.seekg(0);
		file.read(fileBuffer.data(), fileSize);
		file.close();
	}
	
}