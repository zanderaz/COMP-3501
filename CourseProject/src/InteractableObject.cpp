#include "InteractableObject.h"

// constructor
InteractableObject::InteractableObject(const ofMesh& mesh, const glm::vec3& position, float scale, int sp_index) 
	: GameObject(mesh, position, scale) {
	spawn_portal_index = sp_index;
}

// same as normal game object
void InteractableObject::draw(ofShader* lightingShader) {
	GameObject::draw(lightingShader);
}
