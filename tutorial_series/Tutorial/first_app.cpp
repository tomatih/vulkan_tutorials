#include "first_app.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

namespace lve {

void FirstApp::run(){
	// run until window terminated
	while (!lveWindow.shouldClose()) {
		// get glfw window events
		glfwPollEvents();
	}
}

}