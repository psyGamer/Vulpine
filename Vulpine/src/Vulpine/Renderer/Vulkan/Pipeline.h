#pragma once

#include "Core/Base.h"

#include "Vulkan.h"

#include "Shader.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"

namespace Vulpine::Vulkan
{
	class Pipeline
	{
	public:
		Pipeline(const Shader& vertexShader, const Shader& fragmentShader);
		~Pipeline();

		void Create();
		void Destroy();

		void SetVertexBuffer(const VertexBuffer& vertexBuffer);
		void SetIndexBuffer(const IndexBuffer& indexBuffer);
		void SetUniformBuffer(const UniformBuffer& uniformBuffer);

		void ResetViewport();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		VkViewport GetViewport() const { return m_Viewport; }

		void ResetScissor();
		void SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		VkRect2D GetScissor() const { return m_Scissor; }

		Reference<const VertexBuffer> GetVertexBuffer() const { return m_pVertexBuffer; }
		Reference<const IndexBuffer> GetIndexBuffer() const { return m_pIndexBuffer; }
		Reference<const UniformBuffer> GetUniformBuffer() const { return m_pUniformBuffer; }

		const VkPipeline& GetPipeline() const { return m_Pipeline; }
		const VkPipelineLayout& GetPipelineLayout() const { return m_PipelineLayout; }

	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;

		Shader m_VertexShader;
		Shader m_FragmentShader;

		Reference<const VertexBuffer> m_pVertexBuffer;
		Reference<const IndexBuffer> m_pIndexBuffer;
		Reference<const UniformBuffer> m_pUniformBuffer;

		VkViewport m_Viewport;
		VkRect2D m_Scissor;
	};
}

