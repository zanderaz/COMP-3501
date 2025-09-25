#pragma once

#include "GameObject.h"
#include <glm/glm.hpp>

class EnemyGameObject : public GameObject {

public:

	EnemyGameObject(const glm::vec3& position, float scale);
	void update(float deltaTime) override;
	void faceTowards(const glm::vec3& point_to_face);
	void draw() override;

protected:

	// speed constant
	const float ENEMY_SPEED = 50.0f;

};

