#include "first_app.hpp"
#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {

FirstApp::FirstApp(){
	loadModels();
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
		drawFrame();
	}
	// wait for GPU cleanup
	vkDeviceWaitIdle(lveDevice.device());
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

void FirstApp::createCommandBuffers(){
	// make space for as many command buffers as frames in flight
	commandBuffers.resize(lveSwapChain.imageCount());

	VkCommandBufferAllocateInfo alloInfo {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = lveDevice.getCommandPool(),
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()),
	};

	if(vkAllocateCommandBuffers(lveDevice.device(), &alloInfo, commandBuffers.data()) != VK_SUCCESS){
		throw std::runtime_error("Failed to allocate command buffers");
	}

	for(int i=0; i<commandBuffers.size(); i++){
		VkCommandBufferBeginInfo beginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		};

		if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS){
			throw std::runtime_error("Failed to begin command buffer");
		}

		std::array<VkClearValue, 2> clearValues;
		clearValues[0] = {
			.color = {0.1f, 0.1f, 0.1f, 1.0f}
		};
		clearValues[1] = {
			.depthStencil = {1.0f, 0},
		};

		VkRenderPassBeginInfo renderPassInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = lveSwapChain.getRenderPass(),
			.framebuffer = lveSwapChain.getFrameBuffer(i),
			.renderArea {
				.offset = {0,0},
				.extent = lveSwapChain.getSwapChainExtent()
			},
			.clearValueCount = static_cast<uint32_t>(clearValues.size()),
			.pClearValues = clearValues.data()
		};

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		lvePipeline->bind(commandBuffers[i]);
		lveModel->bind(commandBuffers[i]);
		lveModel->draw(commandBuffers[i]);

		vkCmdEndRenderPass(commandBuffers[i]);
		if(vkEndCommandBuffer(commandBuffers[i]) !=VK_SUCCESS){
			throw std::runtime_error("Failed to record command buffer!");
		}
	}

}

void FirstApp::drawFrame(){
	uint32_t imageIndex;
	auto result = lveSwapChain.acquireNextImage(&imageIndex);
	if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if(result != VK_SUCCESS){
		throw std::runtime_error("Failed to present swap chain image!");
	}
}

void FirstApp::loadModels(){
	std::vector<LveModel::Vertex> verticies = {
		{{ 0.0f, -0.5f}},
		{{ 0.5f,  0.5f}},
		{{-0.5f,  0.5f}}
	};

	lveModel = std::make_unique<LveModel>(lveDevice, verticies);

}

}