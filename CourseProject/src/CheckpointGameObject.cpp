#include "CheckpointGameObject.h"


CheckpointGameObject::CheckpointGameObject(const ofMesh& mesh, const glm::vec3& position, float scale)
	: GameObject(mesh, position, scale) {
	// creating a base checkpoint as a sphere
	radius = 15;
	colour = glm::vec3(20.f, 255.f, 20.f);
}


//draw the checkpoints
void CheckpointGameObject::draw(ofShader* lightingShader) {
	GameObject::draw(lightingShader);
}

