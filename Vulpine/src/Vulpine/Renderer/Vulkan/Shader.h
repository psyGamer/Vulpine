#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Shader
	{
	public:
		enum class Type
		{
			VERTEX, FRAGMENT
		};

		Shader(const std::string& filePath, Shader::Type type, const std::string& entryPoint = "main");
		~Shader();

		VkShaderModule GetShaderModule() const { return m_ShaderModule; }
		VkPipelineShaderStageCreateInfo GetShaderStageInfo() const { m_ShaderStageInfo; }

	private:
		VkShaderModule m_ShaderModule;
		VkPipelineShaderStageCreateInfo m_ShaderStageInfo;
	};
}

