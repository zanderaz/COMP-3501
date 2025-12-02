#include "redBloodCell.h"

ParticleSystemHolder::ParticleSystemHolder(ParticleSystem* rbc, const ofMesh& mesh, const glm::vec3& position, float scale)
	: GameObject(mesh, position, scale) {
	//mesh = ofMesh::box(1, 1, 1);
	//mesh = ofMesh::sphere(0.5, 100);
	//mesh = ofMesh::cylinder(0.5, 0.25, 20, 20);
	//position = glm::vec3(0, -1, 0);
	colour = glm::vec3(1.0f, 0.1f, 0.2f);
	this->ps = rbc;
	//rbc->setPosition(position + glm::vec3(0, 0.1, 0));
}

void ParticleSystemHolder::update(float deltaTime) {
	ps->setPosition(this->getPosition());
	ps->update();
}

void ParticleSystemHolder::draw(ofShader* lightingShader) {
	// currently lighting has a bug with non-textured objects
	//lightingShader->setUniformMatrix4f("worldMatrix", this->getWorldMatrix());
	glm::mat4 worldWithScale = this->getWorldMatrix();
	worldWithScale = glm::scale(worldWithScale, glm::vec3(0.8f, 0.4f, 1.0f));
	lightingShader->setUniformMatrix4f("worldMatrix", worldWithScale);
	lightingShader->setUniform3f("objectColor", colour);
	lightingShader->setUniform1i("isLight", false);
	lightingShader->setUniform3f("emissionColor", glm::vec3(0.0));

	ofPushMatrix();
	//ofTranslate(position);
	//ofScale(0.8f, 0.4f, 1.0f);
	//ofSetColor(colour[0], colour[1], colour[2]);
	mesh.draw();
	ps->draw();
	ofPopMatrix();
}