#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class CommandPool
	{
	public:
		CommandPool(VkQueue queue, uint32_t queueIndex, VkCommandPoolCreateFlags flags = 0);
		~CommandPool();

		VkCommandPool GetPool() const { return m_CommandPool; }
		VkQueue GetQueue() const { return m_Queue; }

	private:
		VkCommandPool m_CommandPool;
		VkQueue m_Queue;
	};

	class GraphicsCommandPool
	{
	public:
		static void Create();
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		static std::shared_ptr<const CommandPool> GetPool() { return s_pCommandPool; }

	private:
		static std::shared_ptr<CommandPool> s_pCommandPool;
	};

	class TransferCommandPool
	{
	public:
		static void Create();
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		static std::shared_ptr<const CommandPool> GetPool() { return s_pCommandPool; }

	private:
		static std::shared_ptr<CommandPool> s_pCommandPool;
	};
}
