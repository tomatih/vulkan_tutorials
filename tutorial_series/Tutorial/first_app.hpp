#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"

namespace lve {
class FirstApp{
public:
	// app aprameters
	// size const for now
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	
	FirstApp();
	~FirstApp();

	// deleting copy constructors for memory safety
	FirstApp(const FirstApp&) = delete;
	FirstApp operator=(const FirstApp&) = delete;


	// the application running loop
	void run();
private:
	// our window object created on instance
	LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
	LveDevice lveDevice{lveWindow};
	LveRenderer lveRenderer{lveWindow, lveDevice};
	std::vector<LveGameObject> gameObjects;

	
	void loadGameObjects();
};
}