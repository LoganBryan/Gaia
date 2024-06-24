#include "DeviceManager.h"
#include <stdexcept>

void DeviceManager::Init()
{
	//m_instance = instance;
	//m_surface = surface;
}

std::multimap<int, VkPhysicalDevice> DeviceManager::GetPhysicalDevices()
{
	if (!m_devices.empty())
	{
		return m_devices;
	}

	// Start listing graphics cards
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(CONTEXTMGR->GetVkInstance(), &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("[DeviceSetup] Failed to find GPU(s) that support Vulkan!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(CONTEXTMGR->GetVkInstance(), &deviceCount, devices.data());

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
	if (m_devices.empty())
	{
		GetPhysicalDevices();
	}

	if (m_devices.rbegin()->first > 0)
	{
		m_physicalDevice = m_devices.rbegin()->second;
		m_msaaSamples = GetMaxUsableSampleCount();
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

	QueueFamilyIndicies indices = CONTEXTMGR->FindQueueFamilies(device);
	bool extensionsSupported = CheckDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	int score = 0;

	if (!indices.IsComplete() && !extensionsSupported)
	{
		return 0;
	}

	SwapChainSupportDetails swapChainSupport = CONTEXTMGR->QuerySwapChainSupport(device);
	swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

	if (!swapChainAdequate)
	{
		return 0;
	}

	// Required features of the application
	if (!m_deviceFeatures.geometryShader || !m_deviceFeatures.samplerAnisotropy)
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
	QueueFamilyIndicies indicies = CONTEXTMGR->FindQueueFamilies(m_physicalDevice);
	
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamiles = { indicies.graphicsFamily.value(), indicies.presentFamily.value() };
	
	// Priority to influence command buffer's execution scheduler
	float queuePriority = 1.0f;
	
	for (uint32_t queueFamily : uniqueQueueFamiles)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
	
		queueCreateInfos.push_back(queueCreateInfo);
	}
	
	// Specify device features that will be used
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	
	// Create the logical device
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	
	// Setup device specific extensions and validation layers - primarily for compatability for older versions
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}
	
	if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}
	
	vkGetDeviceQueue(m_device, indicies.graphicsFamily.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, indicies.graphicsFamily.value(), 0, &m_presentQueue);
}

//void DeviceManager::CreateCommandPool()
//{
//	QueueFamilyIndicies queueFamilyIndices = CONTEXTMGR->FindQueueFamilies(m_physicalDevice);
//
//	VkCommandPoolCreateInfo poolInfo{};
//	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
//	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
//
//	if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
//	{
//		throw std::runtime_error("Failed to create command pool!");
//	}
//}
//
//void DeviceManager::CreateCommandBuffers()
//{
//	m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
//
//	VkCommandBufferAllocateInfo allocateInfo{};
//	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//	allocateInfo.commandPool = m_commandPool;
//	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//	allocateInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
//
//	if (vkAllocateCommandBuffers(m_device, &allocateInfo, m_commandBuffers.data()) != VK_SUCCESS)
//	{
//		throw std::runtime_error("Failed to allocate command buffers!");
//	}
//}
//
//void DeviceManager::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, std::vector<VkFramebuffer>& frameBuffers, VkExtent2D extents)
//{
//	VkCommandBufferBeginInfo beginInfo{};
//	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//	beginInfo.flags = 0; // Opt
//	beginInfo.pInheritanceInfo = nullptr; // Opt
//
//	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
//	{
//		throw std::runtime_error("Failed to begin recording command buffer!");
//	}
//
//	// Clear color + depth
//	std::array<VkClearValue, 2> clearValues{};
//	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
//	clearValues[1].depthStencil = { 1.0f, 0 };
//
//	// Start render pass
//	VkRenderPassBeginInfo renderPassInfo{};
//	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//	renderPassInfo.renderPass = renderPass;
//	renderPassInfo.framebuffer = frameBuffers[imageIndex];
//	renderPassInfo.renderArea.offset = { 0,0 };
//	renderPassInfo.renderArea.extent = extents;
//
//	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
//	renderPassInfo.pClearValues = clearValues.data();
//
//	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
//
//	// Bind to graphics pipeline and begin drawing
//	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, PIPELINEMGR->GetGraphicsPipeline());
//
//	// Bind vertex + index buffer
//	VkBuffer 
//
//}

VkSampleCountFlagBits DeviceManager::GetMaxUsableSampleCount()
{
	VkPhysicalDeviceProperties physicalDeviceProps;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &physicalDeviceProps);

	VkSampleCountFlags counts = physicalDeviceProps.limits.framebufferColorSampleCounts & physicalDeviceProps.limits.framebufferDepthSampleCounts;

	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}
