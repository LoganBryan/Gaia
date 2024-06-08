#pragma once
#ifndef PIPELINE_MGR_H
#define PIPELINE_MGR_H

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"
#include <mutex>
#include <string>
#include <vector>

#include "Context.h"

namespace GAIA
{
	class PipelineManager
	{
	public:
		void Init(VkDevice device, VkExtent2D swapChainExtent, VkRenderPass renderPass);
		void Cleanup();

		VkPipeline GetGraphicsPipeline() const { return m_graphicsPipeline; }
		VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }

	public:
		// Assure class can't be cloned or assigned
		PipelineManager(PipelineManager& other) = delete;
		void operator=(const PipelineManager&) = delete;

		static inline PipelineManager* Get()
		{
			std::lock_guard<std::mutex> lock(mutex);
			if (pInstance == nullptr)
			{
				pInstance = new PipelineManager();
			}
			return pInstance;
		}
	private:
		static PipelineManager* pInstance;
		static std::mutex mutex;

		void CreateGraphicsPipeline();
		std::vector<char> ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);

		VkDevice m_device;
		VkPipeline m_graphicsPipeline;
		VkPipelineLayout m_pipelineLayout;

	protected:
		PipelineManager() {}
		~PipelineManager() {}
	};
}

#define PIPELINEMGR (GAIA::PipelineManager::Get())

#endif // PIPELINE_MGR_H
