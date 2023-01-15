#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "lve_device.hpp"

namespace lve {

struct PipelineConfigInfo {
	VkViewport viewPort;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class LvePipeline{
public:
	// simple constructot
	LvePipeline(LveDevice &device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

	~LvePipeline();

	// delete copy constructors to prevent vulkan pointer duplication
	LvePipeline(const LvePipeline&) = delete;
	void operator=(const LvePipeline&) = delete;

	// default config
	static PipelineConfigInfo defaultPipelineConfngInfo(uint32_t width, uint32_t height);


private:
	LveDevice& lveDevice; // allowed only because implicitly devices must outive pipelines
	// Vulkan pointers
	VkPipeline graphicsPipeline;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	// helper to read the compiled shader files
	static std::vector<char> readFile(const std::string& filePath);

	// simple pipeline createion
	void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

	void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
};

}