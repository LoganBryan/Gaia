#pragma once
#include <vulkan/vulkan.h>

namespace GAIA
{
	class DeviceManager
	{
	public:
		void PickPhysicalDevice(VkInstance instance);
		void CreateLogicalDevice(VkSurfaceCapabilitiesKHR surface);
		VkQueue GetGraphicsQueue();
		VkQueue GetPresentQueue();
		void Cleanup();

	private:
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

	};
}
