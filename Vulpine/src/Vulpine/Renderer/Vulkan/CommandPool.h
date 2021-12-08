#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class CommandPool
	{
	public:
		CommandPool(uint32_t queueIndex, VkCommandPoolCreateFlags flags = 0);
		~CommandPool();

		const VkCommandPool& GetPool() const { return m_CommandPool; }

	private:
		VkCommandPool m_CommandPool;
	};

	class GraphicsCommandPool
	{
	public:
		static void Create();
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		static const CommandPool& GetPool() { return *s_pCommandPool; }

	private:
		static std::shared_ptr<CommandPool> s_pCommandPool;
	};

	class TransferCommandPool
	{
	public:
		static void Create();
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		static const CommandPool& GetPool() { return *s_pCommandPool; }

	private:
		static std::shared_ptr<CommandPool> s_pCommandPool;
	};
}
