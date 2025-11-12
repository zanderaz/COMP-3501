#include "PowerUpObject.h"

// constructor, does the same shit as the base class, however might need a different mesh down the road so yeah
PowerUpObject::PowerUpObject(const ofMesh& mesh, const glm::vec3& position, float scale)
	: GameObject(mesh, position, scale) {
	radius = 20;
	colour = glm::vec3(10.f, 180.f, 255.f);
}

// draw with appropriate scale, position, colour, and mesh
void PowerUpObject::draw(void) {
	GameObject::draw();
}