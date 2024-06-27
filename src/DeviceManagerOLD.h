#pragma once
#include "Context.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <string>
#include <optional>

#include "PipelineManager.h" 
#include "Context.h"

class DeviceManager
{
public:
	void Init();

	// Get all devices, sorted in order of suitability - i.e. most features
	std::multimap<int, VkPhysicalDevice> GetPhysicalDevices();
	int RateDeviceSuitability(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	VkPhysicalDevice GetBestPhysicalDevice();

	void CreateLogicalDevice();

	// Move to buffer
	//void CreateCommandPool();
	//void CreateCommandBuffers();
	//void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, std::vector<VkFramebuffer>& frameBuffers, VkExtent2D extents);

public:
	VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
	VkPhysicalDeviceProperties GetDeviceProperties() const { return m_deviceProperties; }
	VkPhysicalDeviceFeatures GetDeviceFeatures() const { return m_deviceFeatures; }

	inline VkDevice GetDevice() { return m_device; }
	inline VkQueue GetGraphicsQueue() { return m_graphicsQueue; }
	inline VkQueue GetPresentQueue() { return m_presentQueue; }
	inline VkSampleCountFlagBits GetSampleCount() { return m_msaaSamples; }

private:
	VkSampleCountFlagBits GetMaxUsableSampleCount();

private:
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	std::multimap<int, VkPhysicalDevice> m_devices;
	VkDevice m_device;

	//VkCommandPool m_commandPool;
	//std::vector<VkCommandBuffer> m_commandBuffers;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	VkPhysicalDeviceProperties m_deviceProperties;
	VkPhysicalDeviceFeatures m_deviceFeatures;

	VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};

