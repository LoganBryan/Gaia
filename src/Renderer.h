#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace GAIA
{
	class Renderer
	{
	public:
		void Init(); // Pipeline and Swapchain (If they're both not singletons)
		void RenderFrame();
		void RecreateSwapChain();
		void Cleanup();

	private:
		// PipelineManager - Probably isn't needed if it's a singleton
		// SwapChainManager - Same with this (But unsure if i'll make them a singleton)
		std::vector<VkCommandBuffer> m_commandBuffers;
	};
}
