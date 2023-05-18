#include "first_app.hpp"
#include "lve_game_object.hpp"
#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {

struct SimplePushConstantData {
	glm::mat2 trasform{1.f};
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

FirstApp::FirstApp(){
	loadGameObjects();
	createPipelineLayout();
	recreateSwapChain();
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

	VkPushConstantRange pushConstantRange{
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
		.offset = 0,
		.size = sizeof(SimplePushConstantData),
	};


	VkPipelineLayoutCreateInfo pipelineLayoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		// passing info like textures, UOB, ect
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 1,
		.pPushConstantRanges = &pushConstantRange,
	};

	if(vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
		throw std::runtime_error("Failed to create pipeline layout");
	}
}

void FirstApp::createPipeline(){
	assert(lveSwapChain != nullptr && "Cannot create pipeline before swap chain");
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	PipelineConfigInfo pipelineConfig{};
	LvePipeline::defaultPipelineConfngInfo(pipelineConfig);
	pipelineConfig.renderPass = lveSwapChain->getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;

	lvePipeline = std::make_unique<LvePipeline>(lveDevice,"shaders/simple_shader.vert.spv","shaders/simple_shader.frag.spv",pipelineConfig);
}

void FirstApp::createCommandBuffers(){
	// make space for as many command buffers as frames in flight
	commandBuffers.resize(lveSwapChain->imageCount());

	VkCommandBufferAllocateInfo alloInfo {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = lveDevice.getCommandPool(),
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()),
	};

	if(vkAllocateCommandBuffers(lveDevice.device(), &alloInfo, commandBuffers.data()) != VK_SUCCESS){
		throw std::runtime_error("Failed to allocate command buffers");
	}

}

void FirstApp::recordCommandBuffer(int imageIndex){
	VkCommandBufferBeginInfo beginInfo {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	};

	if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS){
		throw std::runtime_error("Failed to begin command buffer");
	}

	std::array<VkClearValue, 2> clearValues;
	clearValues[0] = {
		.color = {0.01f, 0.01f, 0.01f, 1.0f}
	};
	clearValues[1] = {
		.depthStencil = {1.0f, 0},
	};

	VkRenderPassBeginInfo renderPassInfo{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = lveSwapChain->getRenderPass(),
		.framebuffer = lveSwapChain->getFrameBuffer(imageIndex),
		.renderArea {
			.offset = {0,0},
			.extent = lveSwapChain->getSwapChainExtent()
		},
		.clearValueCount = static_cast<uint32_t>(clearValues.size()),
		.pClearValues = clearValues.data()
	};

	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewPort{
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width),
		.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f
	};
	VkRect2D scissor{
		.offset{
			.x = 0,
			.y = 0
		},
		.extent = lveSwapChain->getSwapChainExtent()
	};
	vkCmdSetViewport(commandBuffers[imageIndex],0, 1, &viewPort);
	vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

	renderGameObjects(commandBuffers[imageIndex]);

	vkCmdEndRenderPass(commandBuffers[imageIndex]);
	if(vkEndCommandBuffer(commandBuffers[imageIndex]) !=VK_SUCCESS){
		throw std::runtime_error("Failed to record command buffer!");
	}
}

void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer){
	lvePipeline->bind(commandBuffer);

	for(auto& obj: gameObjects){
		obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());
	}

	for(auto& obj: gameObjects){
		SimplePushConstantData push{
			.trasform = obj.transform2d.mat2(),
			.offset = obj.transform2d.translation,
			.color = obj.color,
		};

		vkCmdPushConstants(
			commandBuffer,
			pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
			0, 
			sizeof(SimplePushConstantData), 
			&push
		);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}

}

void FirstApp::recreateSwapChain(){
	auto extent = lveWindow.getExtent();
	// stall if minimized
	while (extent.width == 0 || extent.height == 0) {
		extent = lveWindow.getExtent();
		glfwWaitEvents();
	}
	// wait till current swap chain stops being used
	vkDeviceWaitIdle(lveDevice.device());

	if(lveSwapChain == nullptr){
		lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
	}
	else {
		lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent, std::move(lveSwapChain));
		if(lveSwapChain->imageCount() != commandBuffers.size()){
			freeCommandBuffers();
			createCommandBuffers();
		}
	}
	createPipeline();

}

void FirstApp::drawFrame(){
	uint32_t imageIndex;
	auto result = lveSwapChain->acquireNextImage(&imageIndex);

	if(result == VK_ERROR_OUT_OF_DATE_KHR){
		recreateSwapChain();
		return;
	}

	if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	recordCommandBuffer(imageIndex);
	result = lveSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized()){
		lveWindow.resetWindowResizedFlag();
		recreateSwapChain();
		return;
	}
	if(result != VK_SUCCESS){
		throw std::runtime_error("Failed to present swap chain image!");
	}
}

LveModel::Vertex getMidpoint(LveModel::Vertex a, LveModel::Vertex b){
	return {
		{
			(a.position.x + b.position.x)/2, 
			(a.position.y + b.position.y)/2
		},
		{
			(a.color.r+b.color.r)/2,
			(a.color.g+b.color.g)/2,
			(a.color.b+b.color.b)/2
		}
	};
}

std::vector<LveModel::Vertex> subdivide(std::vector<LveModel::Vertex> original){
	LveModel::Vertex Ap = getMidpoint(original[0], original[1]);
	LveModel::Vertex Bp = getMidpoint(original[1], original[2]);
	LveModel::Vertex Cp = getMidpoint(original[2], original[0]);

	return {original[0],Ap,Cp, Cp,Bp,original[2], Ap,original[1],Bp};
}

std::vector<LveModel::Vertex> generateSierpinski(const std::vector<LveModel::Vertex> &original){
	std::vector<LveModel::Vertex> out(original.size()*3);

	for(int i=0; i<original.size()/3; i++){
		std::vector<LveModel::Vertex> old_triangle = { original[i*3], original[i*3+1], original[i*3+2]};
		auto new_triangle = subdivide(old_triangle);
		for(int j=0; j<new_triangle.size(); j++){
			out[i*9+j] = new_triangle[j];
		}
	}

	return out;
}

void FirstApp::loadGameObjects(){
	std::vector<LveModel::Vertex> verticies = {
		{{ 0.00f, -0.75f},{1.0f, 0.0f, 0.0f}},
		{{ 0.75f,  0.75f},{0.0f, 1.0f, 0.0f}},
		{{-0.75f,  0.75f},{0.0f, 0.0f, 1.0f}}
	};

	/*for(int i=0; i<9; i++){
		verticies = generateSierpinski(verticies);
	}*/

	auto lveModel = std::make_shared<LveModel>(lveDevice, verticies);

	auto triangle = LveGameObject::createGameObject();
	triangle.model = lveModel;
	triangle.color = {.1f, .8f, .1f};
	triangle.transform2d.translation.x = .2f;
	triangle.transform2d.scale = {2.f, .5f};
	triangle.transform2d.rotation = .25f * glm::two_pi<float>();

	gameObjects.push_back(std::move(triangle));

}

void FirstApp::freeCommandBuffers(){
	vkFreeCommandBuffers(lveDevice.device(), lveDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();
}

}