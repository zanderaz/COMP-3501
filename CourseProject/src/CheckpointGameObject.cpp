#include "CheckpointGameObject.h"


CheckpointGameObject::CheckpointGameObject(const ofMesh& mesh, const glm::vec3& position, float scale, const glm::vec3& teleportPosition)
	: GameObject(mesh, position, scale) {
	// creating a base checkpoint as a sphere
	radius = 15;
	colour = glm::vec3(0.1f, 1.0f, 0.1f);

	this->teleportPosition = teleportPosition;
}


//draw the checkpoints
void CheckpointGameObject::draw(ofShader* lightingShader) {
	GameObject::draw(lightingShader);
}

