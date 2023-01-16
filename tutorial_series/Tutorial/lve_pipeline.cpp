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
#include <cassert>
#include <vulkan/vulkan_core.h>

#include "lve_model.hpp"

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

	assert( configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
	assert( configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderPass provided in configInfo");

	// read the compiled shader files
	auto fragCode = readFile(fragFilePath);
	auto vertCode = readFile(vertFilePath);

	// create shaders from that code
	createShaderModule(fragCode, &fragShaderModule);
	createShaderModule(vertCode, &vertShaderModule);

	VkPipelineShaderStageCreateInfo shaderStages[2] = {
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.stage = VK_SHADER_STAGE_VERTEX_BIT,
			.module = vertShaderModule,
			.pName = "main",
		},
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			.module = fragShaderModule,
			.pName = "main",
		}
	};

	auto bindingDescriptions = LveModel::Vertex::getBindingDescriptions();
	auto attributeDescriptions = LveModel::Vertex::getAttributeDescriptions();
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size()),
		.pVertexBindingDescriptions = bindingDescriptions.data(),
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
		.pVertexAttributeDescriptions = attributeDescriptions.data(),
	};

	VkGraphicsPipelineCreateInfo pipelineInfo{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = 2,
		.pStages = shaderStages,
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &configInfo.inputAssemblyInfo,
		.pViewportState = &configInfo.viewportInfo,
		.pRasterizationState = &configInfo.rasterizationInfo,
		.pMultisampleState = &configInfo.multisampleInfo,
		.pDepthStencilState = &configInfo.depthStencilInfo,
		.pColorBlendState = &configInfo.colorBlendInfo,
		.pDynamicState = &configInfo.dynamicStateInfo,
		.layout = configInfo.pipelineLayout,
		.renderPass = configInfo.renderPass,
		.subpass = configInfo.subpass,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1,
	};

	if(vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS){
		throw std::runtime_error("failed to crate graphics pipeline");
	}

}

LvePipeline::LvePipeline(LveDevice &device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo): lveDevice(device){
	createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
}

LvePipeline::~LvePipeline(){
	vkDestroyShaderModule(lveDevice.device(), vertShaderModule, nullptr);
	vkDestroyShaderModule(lveDevice.device(), fragShaderModule, nullptr);

	vkDestroyPipeline(lveDevice.device(), graphicsPipeline, nullptr);
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

void LvePipeline::defaultPipelineConfngInfo(PipelineConfigInfo& configInfo){

	// viewport and scissor package
	configInfo.viewportInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = nullptr,
		.scissorCount = 1,
		.pScissors = nullptr
	};

	// first stage of the pipeline (ordering data into vertexes)
	configInfo.inputAssemblyInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, // interpret every 3 vertices as a triangle
		.primitiveRestartEnable = VK_FALSE // if strip enables mutiple strips from one data 
	};
	//  third stage of the pipeine (vertex to fragment)
	configInfo.rasterizationInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE, // force normalized glPosition
		.rasterizerDiscardEnable = VK_FALSE, // controls if primitves are discarded immideatley before rastarization
		.polygonMode = VK_POLYGON_MODE_FILL, // just points, lines or filled triangles
		.cullMode = VK_CULL_MODE_NONE, // back/front face culling (do we render triangles not facing the camera)
		.frontFace = VK_FRONT_FACE_CLOCKWISE, // what is "facing the camera"
		.depthBiasEnable = VK_FALSE, // alter depth values of vertices
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
		.lineWidth = 1.0f,
	};
	// multisampling config
	configInfo.multisampleInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
		.minSampleShading = 1.0f,
		.pSampleMask = nullptr,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE,
	};
	//color blendining
	configInfo.colorBlendAttachment = {
		.blendEnable = VK_FALSE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
	};
	configInfo.colorBlendInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO, 
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &configInfo.colorBlendAttachment,
		.blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
	};
	// depth buffer
	configInfo.depthStencilInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS,
		.depthBoundsTestEnable = VK_FALSE,
		.stencilTestEnable = VK_FALSE,
		.front = {},
		.back = {},
		.minDepthBounds = 0.0f,
		.maxDepthBounds = 1.0f,
	};
	// dynamic state
	configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	configInfo.dynamicStateInfo = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.flags = 0,
		.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size()),
		.pDynamicStates = configInfo.dynamicStateEnables.data(),
	};
}

void LvePipeline::bind(VkCommandBuffer commandBuffer){
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
};

}