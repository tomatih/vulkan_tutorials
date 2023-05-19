#include "lve_renderer.hpp"
#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>


namespace lve {

LveRenderer::LveRenderer(LveWindow& window, LveDevice& device) : lveWindow(window), lveDevice(device){
	recreateSwapChain();
	createCommandBuffers();
}

LveRenderer::~LveRenderer(){
	freeCommandBuffers();
}


void LveRenderer::createCommandBuffers(){
	// make space for as many command buffers as frames in flight
	commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

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

void LveRenderer::recreateSwapChain(){
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
		std::shared_ptr<LveSwapChain> oldSwapChain = std::move(lveSwapChain);
		lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent, oldSwapChain);

		if(!oldSwapChain->compareSwapChainFormats(*lveSwapChain.get())){
			throw std::runtime_error("Swap chain image(or depth) format channged!");
		}
	}

}

void LveRenderer::freeCommandBuffers(){
	vkFreeCommandBuffers(lveDevice.device(), lveDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();
}


VkCommandBuffer LveRenderer::beginFrame(){
	assert(!isFrameStarted && "Can't call begin when frame already in progress");

	auto result = lveSwapChain->acquireNextImage(&currentImageIndex);

	if(result == VK_ERROR_OUT_OF_DATE_KHR){
		recreateSwapChain();
		return nullptr;
	}

	if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	isFrameStarted = true;

	auto commandBuffer = getCurrentCommandBuffer();

	VkCommandBufferBeginInfo beginInfo {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	};

	if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
		throw std::runtime_error("Failed to begin command buffer");
	}

	return commandBuffer;
}

void LveRenderer::endFrame(){
	assert(isFrameStarted && "Can't end a frame with no frames in progress");

	auto commandBuffer = getCurrentCommandBuffer();
	if(vkEndCommandBuffer(commandBuffer) !=VK_SUCCESS){
		throw std::runtime_error("Failed to record command buffer!");
	}

	auto result = lveSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized()){
		lveWindow.resetWindowResizedFlag();
		recreateSwapChain();
	}
	else if(result != VK_SUCCESS){
		throw std::runtime_error("Failed to present swap chain image!");
	}

	isFrameStarted = false;
	currentFrameIndex = (currentFrameIndex+1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void LveRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){
	assert(isFrameStarted && "Can't begin a render pass with no frames in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin a render pass on a command buffer from a different frame");

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
		.framebuffer = lveSwapChain->getFrameBuffer(currentImageIndex),
		.renderArea {
			.offset = {0,0},
			.extent = lveSwapChain->getSwapChainExtent()
		},
		.clearValueCount = static_cast<uint32_t>(clearValues.size()),
		.pClearValues = clearValues.data()
	};

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

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
	vkCmdSetViewport(commandBuffer,0, 1, &viewPort);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void LveRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){
	assert(isFrameStarted && "Can't end a render pass with no frames in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "Can't end a render pass on a command buffer from a different frame");

	vkCmdEndRenderPass(commandBuffer);
}



}