#pragma once
#ifndef PIPELINE_MGR_H
#define PIPELINE_MGR_H

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include <mutex>
#include <string>
#include <vector>

#include "Context.h"
#include "Singleton.h"

namespace GAIA
{
	class PipelineManager : public Singleton<PipelineManager>
	{
	public:
		inline void Init(VkDevice device, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout)
		{
			m_device = device;
			m_swapChainExtent = swapChainExtent;
			m_renderPass = renderPass;
			m_descriptorSetLayout = descriptorSetLayout;
		}
		void Cleanup();

		VkPipeline GetGraphicsPipeline() const { return m_graphicsPipeline; }
		VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }

	private:
		friend class Singleton<PipelineManager>;

		PipelineManager() {}
		~PipelineManager() {}

	private:
		void CreateGraphicsPipeline();
		std::vector<char> ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);

		VkDevice m_device;
		VkPipeline m_graphicsPipeline;
		VkPipelineLayout m_pipelineLayout;
		VkExtent2D m_swapChainExtent;
		VkRenderPass m_renderPass;
		VkDescriptorSetLayout m_descriptorSetLayout;
	};
}

#define PIPELINEMGR GAIA::PipelineManager::GetInstance()

#endif // PIPELINE_MGR_H
