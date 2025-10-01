#include "AstroidGameObject.h"
AstroidGameObject::AstroidGameObject(const glm::vec3& position, float scale) : GameObject(position, scale) {
	//creating the astroids as cubes
	length = 20;
	mesh = ofMesh::box(length, length, length, scale, scale, scale);
	colour = glm::vec3(100.f, 100.f, 100.f);
}


//for drawing the astroids
void AstroidGameObject::draw() {
	ofPushMatrix();

	ofTranslate(position);

	ofSetColor(colour[0], colour[1], colour[2]);

	mesh.draw();

	ofPopMatrix();
}