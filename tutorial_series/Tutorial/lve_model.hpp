#pragma once

#include <cstdint>
#include <glm/fwd.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "lve_device.hpp"

namespace lve {

class LveModel {
public:

	struct Vertex{
		glm::vec2 position;
		glm::vec3 color;

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	LveModel(LveDevice &device, const std::vector<Vertex> &vertices);
	~LveModel();

	// deleting copy to prevent vulkan object cloning
	LveModel(const LveModel&) = delete;
	LveModel operator=(const LveModel&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	LveDevice& lveDevice; // device reference
	// vertex memory
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	uint32_t vertexCount;

	void createVertexBuffers(const std::vector<Vertex> &vertices);

};

}