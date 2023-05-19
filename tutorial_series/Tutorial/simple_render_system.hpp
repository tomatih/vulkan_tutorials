#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"

namespace lve {
class SimpleRenderSystem{
public:	
	SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass);
	~SimpleRenderSystem();

	// deleting copy constructors for memory safety
	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem operator=(const SimpleRenderSystem&) = delete;

	void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject> &gameObjects);
private:
	// our window object created on instance
	LveDevice &lveDevice;
	std::unique_ptr<LvePipeline> lvePipeline;
	VkPipelineLayout pipelineLayout;

	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);
};
}