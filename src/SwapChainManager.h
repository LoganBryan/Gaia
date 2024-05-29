#pragma once
#include "Context.h"
#include <vulkan/vulkan.h>
#include <algorithm>
#include <vector>

class SwapChainManager
{
public:
	void Init(VkDevice device, VkPhysicalDevice physicalDevice);

	void CreateSwapChain();
	void CleanupSwapChain();
	void RecreateSwapChain();
	void CreateImageViews();
	void CreateFrameBuffers();

public:
	inline VkSwapchainKHR GetSwapChain() const { return m_swapChain; }
	inline std::vector<VkImage> GetSwapChainImages() const { return m_swapChainImages; }
	inline std::vector<VkImageView> GetSwapChainImageViews() const { return m_swapChainImageViews; }
	inline VkFormat GetSwapChainImageFormat() const { return m_swapChainImageFormat; }
	inline VkExtent2D GetSwapChainExtent() const { return m_swapChainExtent; }

	inline VkRenderPass &GetRenderPass() 
	{ 
		return m_renderPass; 
	}
	inline std::vector<VkFramebuffer>& GetSwapChainFrameBuffers() { return m_swapChainFrameBuffers; }
	inline VkFramebuffer GetFrameBuffer(uint32_t index) const { return m_swapChainFrameBuffers[index]; }

private:
	VkDevice m_device;
	VkPhysicalDevice m_physicalDevice;

	VkRenderPass m_renderPass;
	VkSwapchainKHR m_swapChain;
	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;
	std::vector<VkFramebuffer> m_swapChainFrameBuffers;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};

