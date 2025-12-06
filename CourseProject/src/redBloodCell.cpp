#include "redBloodCell.h"

RedBloodCell::RedBloodCell(ParticleSystem* rbc, const ofMesh& mesh, const glm::vec3& position, float scale, float max_lifespan)
	: EnemyGameObject(mesh, position, scale) {

	colour = glm::vec3(1.0f, 0.1f, 0.2f);
	this->ps = rbc;

	setRadius(25.0f);

	lifetime = 0.0f;
	this->max_lifetime = max_lifespan;
	use_lifetime = (max_lifetime > 0.0f);

}

RedBloodCell::~RedBloodCell() {
	delete ps;
}

bool RedBloodCell::isExpired() const {
	return use_lifetime && (lifetime >= max_lifetime);
}

void RedBloodCell::update(float deltaTime) {
	if (use_lifetime) {
		lifetime += deltaTime;
		if (lifetime >= max_lifetime) {
			// stop rendering and collisions once lifespan exceeds max
			setVisible(false);
			setCollidable(false);
			return;
		}
	}

	EnemyGameObject::update(deltaTime);
	ps->setPosition(this->getPosition());
	ps->update();
}

void RedBloodCell::draw(ofShader* lightingShader) {
	if (isExpired()) return;

	glm::mat4 worldWithScale = this->getWorldMatrix();
	worldWithScale = glm::scale(worldWithScale, glm::vec3(0.8f, 0.4f, 1.0f));

	lightingShader->setUniformMatrix4f("worldMatrix", worldWithScale);
	lightingShader->setUniform3f("objectColor", colour);
	lightingShader->setUniform1i("isLight", false);
	lightingShader->setUniform3f("emissionColor", glm::vec3(0.0));

	ofPushMatrix();
	mesh.draw();
	ofPopMatrix();
}

void RedBloodCell::drawParticles() {
	if (isExpired()) return;
	ps->draw();
}