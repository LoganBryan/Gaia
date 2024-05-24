#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include "HelloTriangle.h"

int main()
{
	//glfwInit();

	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//GLFWwindow* window = glfwCreateWindow(800, 600, "Gaia - Vulkan", nullptr, nullptr);

	//uint32_t extensionCount = 0;
	//vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	//std::cout << extensionCount << " extensions supported\n";

	//while (!glfwWindowShouldClose(window))
	//{
	//	glfwPollEvents();
	//}

	//glfwDestroyWindow(window);

	//glfwTerminate();

	//return 0;

	HelloTriangle app;

	try
	{
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}