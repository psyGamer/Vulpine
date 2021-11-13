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

		Shader(Shader::Type type, const std::string& entryPoint = "main");
		Shader(const std::string& filePath, Shader::Type type, const std::string& entryPoint = "main");
		~Shader();

		void Load(const std::string& filePath);
		void Destroy();

		VkShaderModule GetShaderModule() const { return m_ShaderModule; }
		VkPipelineShaderStageCreateInfo GetShaderStageInfo() const { return m_ShaderStageInfo; }

	private:
		std::string m_EntryPoint;

		VkShaderModule m_ShaderModule;
		VkPipelineShaderStageCreateInfo m_ShaderStageInfo;
	};
}

