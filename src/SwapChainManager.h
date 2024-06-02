#pragma once
#include <vulkan/vulkan.h>
#include "Context.h"
#include <algorithm>
#include <functional>
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

	// TEMP until refactor
	inline void SetDepthImage(VkImageView depthImView) { m_depthImageView = depthImView; }
	inline void SetColorImage(VkImageView colorImageView) { m_colorImageView = colorImageView; }

	// Temp for now
	static std::function<void()> OnSwapChainRecreate;

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

	// TEMP
	VkImageView m_depthImageView;
	VkImageView m_colorImageView;

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
};

