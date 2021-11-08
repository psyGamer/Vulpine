#pragma once

namespace Vulpine
{
	class FileUtil
	{
	public:
		static std::vector<char>& ReadFile(const std::string& filePath);
	};
}

