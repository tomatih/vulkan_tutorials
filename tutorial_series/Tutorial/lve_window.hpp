#pragma once

#include <string>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lve {

class LveWindow{
public:
	// simple constructor and desrtuctor
	LveWindow(int w, int h, std::string title);
	~LveWindow();

	// preventing copying to maintain the initialization is allocation philosophy
	LveWindow(const LveWindow &) = delete;
	LveWindow &operator=(const LveWindow &) = delete;

	// glfw window forward
	bool shouldClose(){ return  glfwWindowShouldClose(window);}

	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
	// simple window initialization
	void initWindow();

	// window parameters
	const int width;  // cosnt for now resizing is a headache for later
	const int height; // cosnt for now resizing is a headache for later
	std::string windowName;

	// pointer to the actual window object
	GLFWwindow *window;
};

}