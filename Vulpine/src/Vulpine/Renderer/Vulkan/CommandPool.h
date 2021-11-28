#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class CommandPool
	{
	public:
		CommandPool(VkQueue queue, VkCommandPoolCreateFlags flags = 0);
		~CommandPool();

		const VkCommandPool& GetPool() const { return m_CommandPool; }

	private:
		VkCommandPool m_CommandPool;
	};

	class GraphicsCommandPool
	{
	public:
		static void Create() { s_pCommandPool = CreateReference<CommandPool>(Device::GetGraphicsQueue()); }
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		static const VkCommandPool& GetPool() { return s_pCommandPool->GetPool(); }

	private:
		static Reference<CommandPool> s_pCommandPool;
	};

	class TransferCommandPool
	{
	public:
		static void Create() { s_pCommandPool = CreateReference<CommandPool>(Device::GetTransferQueue()); }
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		static const VkCommandPool& GetPool() { return s_pCommandPool->GetPool(); }

	private:
		static Reference<CommandPool> s_pCommandPool;
	};
}
