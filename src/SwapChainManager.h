#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace GAIA
{
	class SwapChainManager
	{
	public:
		void CreateSwapChain(VkSurfaceKHR surface); // DeviceManager
		void CreateImageViews(VkDevice device);
		std::vector<VkImage>& GetSwapChainImages();
		std::vector<VkImageView>& GetSwapChainImageViews();
		VkFormat GetSwapChainFormat();
		VkExtent2D GetSwapChainExtent();
		void Cleanup();

	private:
		VkSwapchainKHR m_swapChain;
		std::vector<VkImage> m_images;
		std::vector<VkImageView> m_imageViews;
		VkFormat m_format;
		VkExtent2D m_extent;
	};
}
