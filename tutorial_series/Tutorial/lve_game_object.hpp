#pragma once

#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <memory>

#include "lve_model.hpp"

namespace lve {

struct Transform2dComponent{
	glm::vec2 translation{};
	glm::vec2 scale{1.0f, 1.0f};
	float rotation = 0.0f;

	glm::mat2 mat2() {
		glm::mat2 scaleMat{
			{scale.x, 0.0f},
			{0.0f, scale.y}
		};

		const float s = glm::sin(rotation);
		const float c = glm::cos(rotation);
		glm::mat2 rotationMat{
			{c, s},
			{-s, c}
		};
		return rotationMat * scaleMat; 
	}
};

class LveGameObject{
public:
	using id_t = unsigned int;

	std::shared_ptr<LveModel> model;
	glm::vec3 color;
	Transform2dComponent transform2d;

	static LveGameObject createGameObject(){
		static id_t currentId = 0;
		return LveGameObject(currentId++);
	}

	LveGameObject(const LveGameObject&) = delete;
	LveGameObject &operator=(const LveGameObject&) = delete;
	LveGameObject(LveGameObject &&) = default;
	LveGameObject &operator=(LveGameObject &&) = default;

	id_t getId() const {return id;} 


private:
	id_t id;

	LveGameObject(id_t objId) : id{objId} {};
};

}