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

		m_ShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_ShaderStageInfo.pNext = nullptr;
		m_ShaderStageInfo.flags = 0;

		m_ShaderStageInfo.pName = entryPoint.c_str();
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

		std::vector<char> shaderCode = FileUtil::ReadFile(filePath);

		VkShaderModuleCreateInfo shaderCreateInfo{};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		shaderCreateInfo.codeSize = shaderCode.size();
		shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VP_ASSERT_VK(vkCreateShaderModule(Device::GetLogicalDevice(), &shaderCreateInfo, nullptr, &m_ShaderModule), "Failed to create shader module!");

		m_ShaderStageInfo.module = m_ShaderModule;
	}

	Shader::~Shader()
	{
		Destroy();
	}

	Shader::Shader(Shader&& other)
		: m_ShaderModule(std::exchange(other.m_ShaderModule, VK_NULL_HANDLE)),
		m_ShaderStageInfo(std::move(other.m_ShaderStageInfo))
	{ }

	Shader::Shader(const Shader& other)
		: m_ShaderModule(other.m_ShaderModule),
		m_ShaderStageInfo(other.m_ShaderStageInfo),
	{ }

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		this->~Shader();

		m_ShaderModule = std::exchange(other.m_ShaderModule, VK_NULL_HANDLE);
		m_ShaderStageInfo = std::move(other.m_ShaderStageInfo);
	}

	Shader& Shader::operator=(const Shader& other) noexcept
	{
		this->~Shader();

		m_ShaderModule = other.m_ShaderModule;
		m_ShaderStageInfo = other.m_ShaderStageInfo;
	}

	void Shader::Destroy()
	{
		vkDestroyShaderModule(Device::GetLogicalDevice(), m_ShaderModule, nullptr);
	}
}
