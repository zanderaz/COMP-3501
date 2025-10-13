#include "GameObject.h"

GameObject::GameObject(const glm::vec3& position, float scale)
	: position(position), scale(scale) {
	angle = 0.0f;
	orientation = glm::quat(1, 0, 0, 0);
	colour = glm::vec3(255.f);
}
