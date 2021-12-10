#include "vppch.h"
#include "Shader.h"

#include "Device.h"

#include "Utils/FileUtil.h"

namespace Vulpine::Vulkan
{
	Shader::Shader(Shader::Type type, std::string entryPoint)
		: m_EntryPoint(std::move(entryPoint))
	{
		VP_ASSERT(entryPoint.empty(), "Shader entrypoint can't be an empty string!");

		m_ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_ShaderStageInfo.pNext = nullptr;
		m_ShaderStageInfo.flags = 0;

		m_ShaderStageInfo.pName = m_EntryPoint.c_str();
		m_ShaderStageInfo.pSpecializationInfo = nullptr;

		switch (type)
		{
		case Type::VERTEX:
			m_ShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case Type::FRAGMENT:
			m_ShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		}

		m_ShaderModule = VK_NULL_HANDLE;
	}

	Shader::Shader(const std::string& filePath, Shader::Type type, std::string entryPoint)
		:Shader(type, std::move(entryPoint))
	{
		Load(filePath);
	}

	Shader::~Shader()
	{
		Destroy();
	}

	void Shader::Load(const std::string& filePath)
	{
		if (m_ShaderModule != VK_NULL_HANDLE)
			return;

		VP_ASSERT(filePath.empty(), "Shader file path can't be an empty string!");

		std::vector<char> shaderCode = FileUtil::ReadFile(filePath);

		VkShaderModuleCreateInfo shaderCreateInfo{};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		shaderCreateInfo.codeSize = shaderCode.size();
		shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VP_ASSERT_VK(vkCreateShaderModule(Device::GetLogicalDevice(), &shaderCreateInfo, nullptr, &m_ShaderModule), "Failed to create shader module!");

		m_ShaderStageInfo.module = m_ShaderModule;
	}

	void Shader::Destroy()
	{
		vkDestroyShaderModule(Device::GetLogicalDevice(), m_ShaderModule, nullptr);
	}
}
