#include "DeviceManager.h"
#include <stdexcept>

void DeviceManager::Init(VkInstance instance, VkSurfaceKHR surface)
{
	m_instance = instance;
	m_surface = surface;
}

QueueFamilyIndicies DeviceManager::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndicies indices;

	// Retrieve list of queue families
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	// Find at least one queue family that supports 'VK_QUEUE_GRAPHICS_BIT'
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (indices.IsComplete())
		{
			break;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		i++;
	}

	return indices;
}

std::multimap<int, VkPhysicalDevice> DeviceManager::GetPhysicalDevices()
{
	if (!m_devices.empty())
	{
		return m_devices;
	}

	// Start listing graphics cards
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("[DeviceSetup] Failed to find GPU(s) that support Vulkan!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	// Sort by score (features)
	for (const auto& device : devices)
	{
		int score = RateDeviceSuitability(device);
		m_devices.insert(std::make_pair(score, device));
	}

	return m_devices;
}

VkPhysicalDevice DeviceManager::GetBestPhysicalDevice()
{
	if (m_devices.rbegin()->first > 0)
	{
		m_physicalDevice = m_devices.rbegin()->second;
		printf("[DeviceSetup] [%x] %s.%x | api ver %x\n", m_deviceProperties.deviceID, m_deviceProperties.deviceName, m_deviceProperties.driverVersion, m_deviceProperties.apiVersion);
	}
	else
	{
		throw std::runtime_error("[DeviceSetup] Failed to find a suitable GPU!");
	}
}

int DeviceManager::RateDeviceSuitability(VkPhysicalDevice device)
{
	vkGetPhysicalDeviceProperties(device, &m_deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &m_deviceFeatures);
	QueueFamilyIndicies indices = FindQueueFamilies(device);
	bool extensionsSupported = CheckDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	int score = 0;

	if (!indices.IsComplete() && !extensionsSupported)
	{
		return 0;
	}

	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
	swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

	if (!swapChainAdequate)
	{
		return 0;
	}

	// Required features of the application
	if (!m_deviceFeatures.geometryShader)
	{
		return 0;
	}


	// Favour discrete
	if (m_deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000;
	}

	// Favor higher texture size support
	score += m_deviceProperties.limits.maxImageDimension2D;

	return score;
}

bool DeviceManager::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	// Query supported extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	// Check off what our device supports against what we need for the application
	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void DeviceManager::CreateLogicalDevice()
{
}

bool DeviceManager::IsDeviceSuitable(VkPhysicalDevice device)
{
	return false;
}

std::vector<const char*> DeviceManager::GetRequiredExtensions()
{
	return std::vector<const char*>();
}
