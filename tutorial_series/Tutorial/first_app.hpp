#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"

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
	LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};
	//LvePipeline lvePileline{lveDevice,"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv",LvePipeline::defaultPipelineConfngInfo(WIDTH, HEIGHT)};
	std::unique_ptr<LvePipeline> lvePipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;

	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers(){};
	void drawFrame(){};
};
}