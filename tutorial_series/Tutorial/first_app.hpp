#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_model.hpp"

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
	std::unique_ptr<LveSwapChain> lveSwapChain;
	std::unique_ptr<LvePipeline> lvePipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;
	std::unique_ptr<LveModel> lveModel;

	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void freeCommandBuffers();
	void drawFrame();
	void loadModels();
	void recreateSwapChain();
	void recordCommandBuffer(int imageIndex);
};
}