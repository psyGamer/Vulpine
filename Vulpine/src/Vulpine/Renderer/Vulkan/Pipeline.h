#pragma once

#include "Vulkan.h"

namespace Vulpine::Vulkan
{
	class Shader;

	class Pipeline
	{
	public:
		static void Create();
		static void Destroy();

	private:
		static VkPipeline s_Pipeline;
		static VkPipelineLayout s_PipelineLayout;

		static Shader s_VertexShader;
		static Shader s_FragmentShader;

		static VertexBuffer s_VertexBufffer;

		static VkViewport s_Viewport;
		static VkRect2D s_Scissor;
	};
}

