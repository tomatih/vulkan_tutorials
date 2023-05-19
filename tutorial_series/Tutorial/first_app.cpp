#include "first_app.hpp"
#include "lve_game_object.hpp"
#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "simple_render_system.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
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

FirstApp::FirstApp(){
	loadGameObjects();
}

FirstApp::~FirstApp(){
}

void FirstApp::run(){
	SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};

	// run until window terminated
	while (!lveWindow.shouldClose()) {
		// get glfw window events
		glfwPollEvents();
		if(auto commandBuffer = lveRenderer.beginFrame()){
			lveRenderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
			lveRenderer.endSwapChainRenderPass(commandBuffer);
			lveRenderer.endFrame();
		}
	}
	// wait for GPU cleanup
	vkDeviceWaitIdle(lveDevice.device());
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

	for(size_t i=0; i<original.size()/3; i++){
		std::vector<LveModel::Vertex> old_triangle = { original[i*3], original[i*3+1], original[i*3+2]};
		auto new_triangle = subdivide(old_triangle);
		for(size_t j=0; j<new_triangle.size(); j++){
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


}