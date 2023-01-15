#include "first_app.hpp"
#include "lve_pipeline.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace lve {

FirstApp::FirstApp(){
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}

FirstApp::~FirstApp(){
	vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

void FirstApp::run(){
	// run until window terminated
	while (!lveWindow.shouldClose()) {
		// get glfw window events
		glfwPollEvents();
	}
}

void FirstApp::createPipelineLayout(){
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		// passing info like textures, UOB, ect
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr,
	};

	if(vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
		throw std::runtime_error("Failed to create pipeline layout");
	}
}

void FirstApp::createPipeline(){
	auto pipelineConfig = LvePipeline::defaultPipelineConfngInfo(lveSwapChain.width(), lveSwapChain.height());
	pipelineConfig.renderPass = lveSwapChain.getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;

	lvePipeline = std::make_unique<LvePipeline>(lveDevice,"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv",pipelineConfig);
}

}