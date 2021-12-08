#pragma once

#include "Vulkan.h"

#include "Buffer.h"

namespace Vulpine::Vulkan
{
	class DescriptorPool
	{
	public:
		static void Create();
		static void Destroy();

		static VkDescriptorPool GetPool() { return s_Pool; }

	private:
		static VkDescriptorPool s_Pool;
	};

	class DescriptorSetLayout
	{
	public:
		static void Create();
		static void Destroy();

		static VkDescriptorSetLayout GetSetLayout() { return s_SetLayout; }

	private:
		static VkDescriptorSetLayout s_SetLayout;
	};

	class DescriptorSet
	{
	public:
		static void Create(const std::vector<VkDescriptorSetLayout>& layouts);

		static std::vector<VkDescriptorSet> GetSets() { return s_Sets; }

	private:
		static std::vector<VkDescriptorSet> s_Sets;
	};

	class UniformBuffer
	{
	public:
		UniformBuffer(size_t bufferSize);

		void SetData(const void* const data);

		std::vector<std::shared_ptr<Buffer>> GetBuffers() const { return m_pBuffers; }
		std::vector<VkDescriptorSet> GetSets() const { return m_pSets; }

	private:
		std::vector<std::shared_ptr<Buffer>> m_pBuffers;
		std::vector<VkDescriptorSet> m_pSets;
	};
}
