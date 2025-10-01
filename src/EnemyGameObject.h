#pragma once

#include "GameObject.h"
#include <glm/glm.hpp>

class EnemyGameObject : public GameObject {

public:

	EnemyGameObject(const glm::vec3& position, float scale);
	inline const float getRadius() const { return radius; }
	inline void setRadius(float r) { radius = r; }
	void update(float deltaTime) override;
	void faceTowards(const glm::vec3& point_to_face);
	void draw() override;

protected:

	// speed constant
	const float ENEMY_BASE_SPEED = 75.0f;
	float enemy_speed;
	float radius;

};

