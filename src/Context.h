#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include <mutex>
#include <iostream>
#include <stdexcept>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif // NDEBUG

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
const std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain" };
const std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

struct QueueFamilyIndicies
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

namespace GAIA
{
	class Context
	{
	public:
		void InitWindow(int width, int height);
		void CreateSurface();
		void CreateInstance();
		void CleanupInstance();
		VkInstance GetVkInstance() const { return m_VkInstance; }

		void SetupDebugMessenger();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	public:
		// Assure class can't be cloned or assigned
		Context(Context& other) = delete;
		void operator=(const Context&) = delete;

		static inline Context* Get()
		{
			std::lock_guard<std::mutex> lock(mutex);
			if (pInstance == nullptr)
			{
				pInstance = new Context();
			}
			return pInstance;
		}

		QueueFamilyIndicies& FindQueueFamilies(VkPhysicalDevice device);

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();

		inline VkSurfaceKHR GetSurface() { return m_surface; }
		inline bool GetFrameResized() { return m_frameBufferResized; }
		inline GLFWwindow* GetWindow() { return m_window; }

		inline void SetFrameResized(bool state) { m_frameBufferResized = state; }

	private:
		static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);

		VkDebugUtilsMessengerEXT m_debugMessenger;

		// Messenger
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		// Debug messenger to be passed into this object
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	private:
		static Context* pInstance;
		static std::mutex mutex;

		GLFWwindow* m_window;

		// Window surface
		VkSurfaceKHR m_surface;

		VkInstance m_VkInstance;

		bool m_frameBufferResized = false;

	protected:
		Context() {}
		~Context() {}
	};
}

#define CONTEXTMGR (GAIA::Context::Get())

#endif // CONTEXT_H