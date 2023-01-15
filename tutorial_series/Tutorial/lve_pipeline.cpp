#include "lve_pipeline.hpp"
#include <cstddef>
#include <cstdint>
#include <ios>
#include <ostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vulkan/vulkan_core.h>

namespace lve {

std::vector<char> LvePipeline::readFile(const std::string& fileName){
	// open file in bbinary mode and go to end
	std::ifstream file{fileName, std::ios::ate | std::ios::binary};

	if(!file.is_open()){
		throw std::runtime_error("Failed to open file: "+fileName);
	}

	// get file size from cursor postiotion
	size_t file_size = static_cast<size_t>(file.tellg());

	// prepare the data buffer and read the data
	std::vector<char> buffer(file_size);
	file.seekg(0);
	file.read(buffer.data(), file_size);
	// close the file
	file.close();

	return buffer;
}

void LvePipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo){
	// read the compiled shader files
	auto fragCode = readFile(fragFilePath);
	auto vertCode = readFile(vertFilePath);

	std::cout << "Vertex shader size: " << vertCode.size()<<std::endl;
	std::cout << "Fragment shader size: " << fragCode.size()<<std::endl;
}

LvePipeline::LvePipeline(LveDevice &device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo): lveDevice(device){
	createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
}

void LvePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule){
	VkShaderModuleCreateInfo createInfo {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	if(vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule)){
		throw std::runtime_error("Failed to create shader module");
	}

}

PipelineConfigInfo LvePipeline::defaultPipelineConfngInfo(uint32_t width, uint32_t height){
	PipelineConfigInfo configInfo{};

	return configInfo;
}

}