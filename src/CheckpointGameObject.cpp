#include "CheckpointGameObject.h"


CheckpointGameObject::CheckpointGameObject(const glm::vec3& position, float scale)  : GameObject(position, scale){
	//creating a base checkpoint as a sphere
	radius = 15;
	mesh = ofMesh::sphere(radius);
	colour = glm::vec3(100.f, 255.f, 80.f);
}


//draw the checkpoints
void CheckpointGameObject::draw() {
	ofPushMatrix();

	ofTranslate(position);

	ofSetColor(colour[0], colour[1], colour[2]);

	mesh.draw();

	ofPopMatrix();
}

