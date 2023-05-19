#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <cassert>
#include <vulkan/vulkan_core.h>

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_swap_chain.hpp"

namespace lve {
class LveRenderer{
public:
	LveRenderer(LveWindow& lveWindow, LveDevice& lveDevice);
	~LveRenderer();

	// deleting copy constructors for memory safety
	LveRenderer(const LveRenderer&) = delete;
	LveRenderer operator=(const LveRenderer&) = delete;

	VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); };
	bool isFrameInProgress() const { return isFrameStarted; };
	VkCommandBuffer getCurrentCommandBuffer() const { 
		assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
		return commandBuffers[currentFrameIndex]; 
	};
	int getFrameIndex() const {
		assert(isFrameStarted && "Cannot get frame index when frame not in progress");
		return currentFrameIndex;
	};


	VkCommandBuffer beginFrame();
	void endFrame();
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
	// our window object created on instance
	LveWindow& lveWindow;
	LveDevice& lveDevice;
	std::unique_ptr<LveSwapChain> lveSwapChain;
	std::vector<VkCommandBuffer> commandBuffers;
	uint32_t currentImageIndex;
	int currentFrameIndex = 0;
	bool isFrameStarted = false;

	void createCommandBuffers();
	void freeCommandBuffers();
	void recreateSwapChain();
};
}