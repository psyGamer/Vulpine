#pragma once

#include "Vulkan.h"

#include "Device.h"

namespace Vulpine::Vulkan
{
	class Instance
	{
	public:
		static const Reference<Instance> GetOrCreate()
		{
			if (!s_Instance)
				s_Instance = CreateReference<Instance>();

			return s_Instance;
		}
		static void Destory()
		{
			if (s_Instance)
				s_Instance->DestoryInstance();
		}

		Instance();

		const Reference<const Device> GetDevice() const { return m_Device; }

	private:
		void DestoryInstance();

	private:
		static Reference<Instance> s_Instance;

		const Reference<const Device> m_Device;

		const Reference<VkInstance> m_Instance;
	};
}