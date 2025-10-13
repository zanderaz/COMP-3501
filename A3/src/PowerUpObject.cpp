#include "PowerUpObject.h"

// constructor, does the same shit as the base class, however might need a different mesh down the road so yeah
PowerUpObject::PowerUpObject(const glm::vec3& position, float scale)
	: GameObject(position, scale) {
	radius = 20;
	mesh = ofMesh::sphere(radius);
	colour = glm::vec3(10.f, 180.f, 255.f);
}

// draw with appropriate scale, position, colour, and mesh
void PowerUpObject::draw(void) {
	ofPushMatrix();
	ofTranslate(position);
	ofSetColor(colour[0], colour[1], colour[2]);
	mesh.draw();
	ofPopMatrix();
}