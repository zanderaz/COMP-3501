#include "redBloodCell.h"

RedBloodCell::RedBloodCell(ParticleSystem* rbc, const ofMesh& mesh, const glm::vec3& position, float scale)
	: GameObject(mesh, position, scale) {

	colour = glm::vec3(1.0f, 0.1f, 0.2f);
	this->ps = rbc;

}

RedBloodCell::~RedBloodCell() {
	delete ps;
}

void RedBloodCell::update(float deltaTime) {
	ps->setPosition(this->getPosition());
	ps->update();
}

void RedBloodCell::draw(ofShader* lightingShader) {

	glm::mat4 worldWithScale = this->getWorldMatrix();
	worldWithScale = glm::scale(worldWithScale, glm::vec3(0.8f, 0.4f, 1.0f));
	lightingShader->setUniformMatrix4f("worldMatrix", worldWithScale);
	lightingShader->setUniform3f("objectColor", colour);
	lightingShader->setUniform1i("isLight", false);
	lightingShader->setUniform3f("emissionColor", glm::vec3(0.0));

	ofPushMatrix();
	mesh.draw();
	ps->draw();
	ofPopMatrix();
}