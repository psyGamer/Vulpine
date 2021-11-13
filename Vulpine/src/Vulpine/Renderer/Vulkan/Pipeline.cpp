#include "vppch.h"
#include "Pipeline.h"

#include "Core/Window.h"

#include "Device.h"
#include "RenderPass.h"

namespace Vulpine::Vulkan
{
	Pipeline::Pipeline(const Shader& vertexShader, const Shader& fragmentShader, const VertexBuffer& vertexBuffer)
		: m_VertexShader(vertexShader), m_FragmentShader(fragmentShader), m_VertexBuffer(vertexBuffer)
	{
		m_Pipeline = VK_NULL_HANDLE;
		m_PipelineLayout = VK_NULL_HANDLE;

		m_Viewport = {};
		m_Scissor = {};
	}

	Pipeline::~Pipeline()
	{
		Destroy();
	}

	void Pipeline::Create()
	{
		// Shader Stages
		std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos = {
			m_VertexShader.GetShaderStageInfo(), m_FragmentShader.GetShaderStageInfo()
		};
		
		// Vertex Input
		//auto bindingDescription = m_VertexBuffer.QueryBindingDescriptions();
		//auto attributeDescriptions = m_VertexBuffer.QueryAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		//vertexInputInfo.vertexBindingDescriptionCount = 1;
		//vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		//vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		//vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;

		// Input Assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Viewport & Scissors
		VkPipelineViewportStateCreateInfo viewportStateInfo{};
		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

		viewportStateInfo.viewportCount = 1;
		viewportStateInfo.pViewports = &m_Viewport;
		viewportStateInfo.scissorCount = 1;
		viewportStateInfo.pScissors = &m_Scissor;

		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

		rasterizer.lineWidth = 1.0f;

		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;

		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;

		// Color Blending
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		// Pipeline Layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		VP_ASSERT_VK(vkCreatePipelineLayout(Device::GetLogicalDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout), "Failed to create pipeline layout!");

		// Dynamic States
		std::vector dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		
		dynamicStateInfo.dynamicStateCount = dynamicStates.size();
		dynamicStateInfo.pDynamicStates = dynamicStates.data();

		// Graphics Pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		pipelineInfo.stageCount = shaderStageInfos.size();
		pipelineInfo.pStages = shaderStageInfos.data();
		
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportStateInfo;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicStateInfo;

		pipelineInfo.layout = m_PipelineLayout;

		pipelineInfo.renderPass = RenderPass::GetRenderPass();
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		VP_ASSERT_VK(vkCreateGraphicsPipelines(Device::GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline), "Failed to create graphics pipeline!");
	}

	void Pipeline::Destroy()
	{
		vkDestroyPipeline(Device::GetLogicalDevice(), m_Pipeline, nullptr);
		vkDestroyPipelineLayout(Device::GetLogicalDevice(), m_PipelineLayout, nullptr);
	}

	void Pipeline::ResetViewport()
	{
		SetViewport(0, 0, Window::GetWidth(), Window::GetHeight());
	}

	void Pipeline::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		m_Viewport.x = x;
		m_Viewport.y = y;
		m_Viewport.width = width;
		m_Viewport.height = height;
	}

	void Pipeline::ResetScissor()
	{
		SetScissor(0, 0, Window::GetWidth(), Window::GetHeight());
	}

	void Pipeline::SetScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		m_Scissor.offset.x = x;
		m_Scissor.offset.y = y;
		m_Scissor.extent.width = width;
		m_Scissor.extent.height = height;
	}

}