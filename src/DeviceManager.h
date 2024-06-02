#pragma once
#include "Context.h"

#include <vulkan/vulkan.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <optional>

//#include "Context.h"

VkSampleCountFlagBits avaialbeSampleCounts[] = {
	VK_SAMPLE_COUNT_64_BIT,
	VK_SAMPLE_COUNT_32_BIT,
	VK_SAMPLE_COUNT_16_BIT,
	VK_SAMPLE_COUNT_8_BIT,
	VK_SAMPLE_COUNT_4_BIT,
	VK_SAMPLE_COUNT_2_BIT
};

class DeviceManager
{
public:
	void Init(VkInstance instance, VkSurfaceKHR surface);

	// Get all devices, sorted in order of suitability - i.e. most features
	std::multimap<int, VkPhysicalDevice> GetPhysicalDevices();
	int RateDeviceSuitability(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	VkPhysicalDevice GetBestPhysicalDevice();

	void CreateLogicalDevice();
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
	//VkInstance m_instance;
	//VkSurfaceKHR m_surface;

	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	std::multimap<int, VkPhysicalDevice> m_devices;
	VkDevice m_device;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	VkPhysicalDeviceProperties m_deviceProperties;
	VkPhysicalDeviceFeatures m_deviceFeatures;

	VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;

};

