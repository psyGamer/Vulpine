#pragma once

namespace Vulpine
{
	class FileUtil
	{
	public:
		static void ReadFile(const std::string& filePath, std::vector<char>& fileBuffer);
	};
}

