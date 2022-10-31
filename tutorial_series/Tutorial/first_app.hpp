#pragma once

#include "lve_window.hpp"

namespace lve {
class FirstApp{
public:
	// app aprameters
	// size const for now
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	
	// the application running loop
	void run();
private:
	// our window object created on instance
	LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
};
}