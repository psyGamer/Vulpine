#include "vppch.h"
#include "Shader.h"

#include "Device.h"

#include "Utils/FileUtil.h"

namespace Vulpine::Vulkan
{
	Shader::Shader(const std::string& filePath, Shader::Type type, const std::string& entryPoint)
	{
		VP_ASSERT(filePath.empty(), "Shader file path can't be an empty string!");
		VP_ASSERT(entryPoint.empty(), "Shader entrypoint can't be an empty string!");

		auto shaderCode = FileUtil::ReadFile(filePath);

		// ShaderModule
		VkShaderModuleCreateInfo shaderCreateInfo{};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		shaderCreateInfo.codeSize = shaderCode.size();
		shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VP_ASSERT_VK(vkCreateShaderModule(Device::GetLogicalDevice(), &shaderCreateInfo, nullptr, &m_ShaderModule), "Failed to create shader module!");

		// ShaderStateInfo
		m_ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_ShaderStageInfo.module = m_ShaderModule;
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
	}

	Shader::~Shader()
	{
		vkDestroyShaderModule(Device::GetLogicalDevice(), m_ShaderModule, nullptr);
	}
}
