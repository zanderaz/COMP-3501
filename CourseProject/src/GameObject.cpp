#include "GameObject.h"

GameObject::GameObject(const ofMesh& mesh, const glm::vec3& position, float scale)
	: mesh(mesh), position(position), scale(scale) {
	orientation = glm::quat(1, 0, 0, 0);
	colour = glm::vec3(255, 255, 255);
}

// universal draw function
void GameObject::draw(ofShader* lightingShader) {
	glm::mat4 world = getWorldMatrix();

	lightingShader->setUniformMatrix4f("worldMatrix", world);
	lightingShader->setUniform3f("objectColor", colour);
	lightingShader->setUniform1i("isLight", false);
	lightingShader->setUniform3f("emissionColor", glm::vec3(0.0));
	// std::cout << "Setting objectColor: " << colour.r << ", " << colour.g << ", " << colour.b << std::endl;

	ofPushMatrix();
	//ofTranslate(position);
	//ofSetColor(colour[0], colour[1], colour[2]);
	mesh.draw();
	ofPopMatrix();
}

glm::mat4 GameObject::getWorldMatrix() {
	glm::mat4 mat(1.0f);

	// Translate
	mat = glm::translate(mat, position);

	// Rotate (quaternion -> matrix)
	mat *= glm::toMat4(orientation);

	// Scale
	mat = glm::scale(mat, glm::vec3(scale));

	return mat;
}
