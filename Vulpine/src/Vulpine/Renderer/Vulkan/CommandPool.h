#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class CommandPool
	{
	public:
		CommandPool(VkQueue queue, uint32_t queueIndex, VkCommandPoolCreateFlags flags = 0);
		~CommandPool();

		CommandPool(CommandPool&& other) noexcept;
		CommandPool(const CommandPool& other) noexcept;
		CommandPool& operator=(CommandPool&& other) noexcept;
		CommandPool& operator=(const CommandPool& other) noexcept;

		inline VkCommandPool GetPool() const { return m_CommandPool; }
		inline VkQueue GetQueue() const { return m_Queue; }

	private:
		VkCommandPool m_CommandPool;
		VkQueue m_Queue;
	};

	class GraphicsCommandPool
	{
	public:
		static void Create();
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		inline static std::shared_ptr<const CommandPool> GetPool() { return s_pCommandPool; }

	private:
		GraphicsCommandPool();

	private:
		static std::shared_ptr<CommandPool> s_pCommandPool;
	};

	class TransferCommandPool
	{
	public:
		static void Create();
		static void Destroy() { s_pCommandPool->~CommandPool(); }

		inline static std::shared_ptr<const CommandPool> GetPool() { return s_pCommandPool; }

	private:
		TransferCommandPool();

	private:
		static std::shared_ptr<CommandPool> s_pCommandPool;
	};
}
