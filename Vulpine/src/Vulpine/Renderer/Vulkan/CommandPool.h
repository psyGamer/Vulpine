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
		static void Create() { s_pCommandPool = std::make_shared<CommandPool>(Device::GetGraphicsQueue()); }
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		static const VkCommandPool& GetPool() { return s_pCommandPool->GetPool(); }

	private:
		static std::shared_ptr<CommandPool> s_pCommandPool;
	};

	class TransferCommandPool
	{
	public:
		static void Create() { s_pCommandPool = std::make_shared<CommandPool>(Device::GetTransferQueue()); }
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		static const VkCommandPool& GetPool() { return s_pCommandPool->GetPool(); }

	private:
		static std::shared_ptr<CommandPool> s_pCommandPool;
	};
}
