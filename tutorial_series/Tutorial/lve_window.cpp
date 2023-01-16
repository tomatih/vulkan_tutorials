#include "lve_window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace lve {

LveWindow::LveWindow(int w, int h, std::string title): width{w},height{h}, windowName{title}{
	// start the window
	initWindow();
}

LveWindow::~LveWindow(){
	// glwf window cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
}

void LveWindow::initWindow(){
	// initialise glfw with vulkan context
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	// initialise the actul window
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCalllback);
}

void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
	if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
		throw std::runtime_error("Failed to create window surface");
	}
}

void LveWindow::framebufferResizeCalllback(GLFWwindow *window, int width, int height){
	auto lveWindow = reinterpret_cast<LveWindow *>(glfwGetWindowUserPointer(window));
	lveWindow->framebufferResised = true;
	lveWindow->width = width;
	lveWindow->height = height;
}

}