#pragma once

#include <cstdint>
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
	VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; };
	bool wasWindowResized() {return framebufferResised;};
	void resetWindowResizedFlag() {framebufferResised = false;};

	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
	// simple window initialization
	void initWindow();
	static void framebufferResizeCalllback(GLFWwindow *window, int width, int height);

	// window parameters
	int width;  // cosnt for now resizing is a headache for later
	int height; // cosnt for now resizing is a headache for later
	bool framebufferResised = false;
	std::string windowName;

	// pointer to the actual window object
	GLFWwindow *window;
};

}