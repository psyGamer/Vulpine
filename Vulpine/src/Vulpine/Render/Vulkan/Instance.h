#pragma once

#include "Vulkan.h"

#include "Device.h"

namespace Vulpine::Vulkan
{
	class Instance
	{
	public:
		static const Reference<const Instance> GetOrCreate()
		{
			if (!s_Instance)
				s_Instance = CreateReference<const Instance>();

			return s_Instance;
		}
		static void Destory()
		{
			if (s_Instance)
				s_Instance->Destory();
		}

		const Reference<const Device> GetDevice() const { return m_Device; }
		
	private:
		Instance();
		
		void Destory();

	private:
		static Reference<const Instance> s_Instance;

		const Reference<const Device> m_Device;

		const Reference<VkInstance> m_Instance;
	};
}