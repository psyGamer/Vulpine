#pragma once

#include "Vulkan.h"

#include "Shader.h"
#include "VertexBuffer.h"

namespace Vulpine::Vulkan
{
	class Pipeline
	{
	public:
		Pipeline(const Shader& vertexShader, const Shader& fragmentShader, const VertexBuffer& vertexBuffer);
		~Pipeline();

		void Create();
		void Destroy();

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;

		Shader m_VertexShader;
		Shader m_FragmentShader;

		VertexBuffer m_VertexBuffer;

		VkViewport m_Viewport;
		VkRect2D m_Scissor;
	};
}

