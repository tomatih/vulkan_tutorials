#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"

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
	LveDevice lveDevice{lveWindow};
	LvePipeline lvePileline{lveDevice,"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv",LvePipeline::defaultPipelineConfngInfo(WIDTH, HEIGHT)};
};
}