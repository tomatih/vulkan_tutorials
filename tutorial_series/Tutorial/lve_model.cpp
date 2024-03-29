#include "lve_model.hpp"
#include "lve_device.hpp"
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve{
	LveModel::LveModel(LveDevice &device, const std::vector<Vertex> &vertices) : lveDevice(device){
		createVertexBuffers(vertices);
	}

	LveModel::~LveModel(){
		vkDestroyBuffer(lveDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(lveDevice.device(), vertexBufferMemory, nullptr);
	}

	void LveModel::createVertexBuffers(const std::vector<Vertex> &vertices){
		// count vertices 
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must me at least 3");
		// calculate buffer size
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		// create buffer
		lveDevice.createBuffer(
			bufferSize, 
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			vertexBuffer, 
			vertexBufferMemory
		);
		// memory mapping
		void *data;
		vkMapMemory(lveDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		// memory copy
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		// unmap
		vkUnmapMemory(lveDevice.device(), vertexBufferMemory);
	}

	void LveModel::draw(VkCommandBuffer commandBuffer){
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void LveModel::bind(VkCommandBuffer commandBuffer){
		VkBuffer buffers[] = {vertexBuffer};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	std::vector<VkVertexInputBindingDescription> LveModel::Vertex::getBindingDescriptions(){
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0] = {
			.binding = 0,
			.stride = sizeof(Vertex),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		};
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions(){
		return {
			{
				.location = 0,
				.binding = 0,
				.format = VK_FORMAT_R32G32_SFLOAT,
				.offset = offsetof(Vertex, position),
			},
			{
				.location = 1,
				.binding = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof(Vertex, color),
			}
		};
	}

}