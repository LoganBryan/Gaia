#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <optional>

const std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain" };

struct QueueFamilyIndicies
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class DeviceManager
{
public:
	void Init(VkInstance instance, VkSurfaceKHR surface);
	QueueFamilyIndicies FindQueueFamilies(VkPhysicalDevice device);

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

private:
	VkInstance m_instance;
	VkSurfaceKHR m_surface;

	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	std::multimap<int, VkPhysicalDevice> m_devices;
	VkDevice m_device;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	VkPhysicalDeviceProperties m_deviceProperties;
	VkPhysicalDeviceFeatures m_deviceFeatures;

	bool IsDeviceSuitable(VkPhysicalDevice device);
	std::vector<const char*> GetRequiredExtensions();
};

