#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace GAIA
{
	class PipelineConfig
	{
	public:
		void InitDefaults();
		void SetShaderStages(std::vector<VkPipelineShaderStageCreateInfo> shaders);
		void SetVertexInputInfo(VkPipelineVertexInputStateCreateInfo info);
		void SetInputAssembly(VkPipelineInputAssemblyStateCreateInfo info);
		void SetViewportState(VkPipelineViewportStateCreateInfo info);
		void SetRasterizer(VkPipelineRasterizationStateCreateInfo info);
		void SetMultisampling(VkPipelineMultisampleStateCreateInfo info);
		void SetColorBlendAttachment(VkPipelineColorBlendAttachmentState info);
		void SetColorBlending(VkPipelineColorBlendStateCreateInfo info);
		void SetDepthStencil(VkPipelineDepthStencilStateCreateInfo info);
		void SetDynamicState(VkPipelineDynamicStateCreateInfo info);
		void SetPipelineLayout(VkPipelineLayout info);
		void SetRenderPass(VkRenderPass render);
		void SetSubpass(uint32_t subpass);

	private:
		std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
		VkPipelineVertexInputStateCreateInfo m_vertexInputInfo;
		VkPipelineInputAssemblyStateCreateInfo m_inputAssembly;
		VkPipelineViewportStateCreateInfo m_viewportState;
		VkPipelineRasterizationStateCreateInfo m_rasterizer;
		VkPipelineMultisampleStateCreateInfo m_multisampler;
		VkPipelineColorBlendAttachmentState m_colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo m_colorBlending;
		VkPipelineDepthStencilStateCreateInfo vk_depthStencil;
		VkPipelineDynamicStateCreateInfo m_dynamicState;
		VkPipelineLayout m_pipelineLayout;
		VkRenderPass m_renderPass;
		uint32_t m_subpass;
	};
}

