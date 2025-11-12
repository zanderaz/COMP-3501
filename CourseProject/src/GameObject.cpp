#include "GameObject.h"

GameObject::GameObject(const ofMesh& mesh, const glm::vec3& position, float scale)
	: mesh(mesh), position(position), scale(scale) {
	orientation = glm::quat(1, 0, 0, 0);
	colour = glm::vec3(255.f);
}

// universal draw function
void GameObject::draw(void) {
	ofPushMatrix();

	ofTranslate(position);
	ofSetColor(colour[0], colour[1], colour[2]);
	mesh.draw();

	ofPopMatrix();
}
