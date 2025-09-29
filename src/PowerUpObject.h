#pragma once

#include "GameObject.h"

class PowerUpObject : public GameObject {

public:

	PowerUpObject(const glm::vec3& position, float scale);
	inline const float getRadius() const { return radius; }
	inline void setRadius(float r) { radius = r; }
	void draw() override;

private:

	float radius;

};

