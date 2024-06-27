#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <string>

namespace GAIA
{
	class Context
	{
	public:
		void InitWindow(int width, int height, std::string title);
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface();
		void Cleanup();

	private:
		GLFWwindow* m_window;
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		// DeviceManager
		// SwapChainManager (if not singletons)

	};
}
