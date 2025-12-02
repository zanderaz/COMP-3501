#include "GameObject.h"

GameObject::GameObject(const ofMesh& mesh, const glm::vec3& position, float scale)
	: mesh(mesh), position(position), scale(scale) {
	orientation = glm::quat(1, 0, 0, 0);
	colour = glm::vec3(0.5f); // default grey color
	visible = true;
	collidable = true;
	computeLocalBounds();
}

// universal draw function
void GameObject::draw(ofShader* lightingShader) {
	glm::mat4 world = getWorldMatrix();
	if (!visible) return;

	lightingShader->setUniformMatrix4f("worldMatrix", world);
	lightingShader->setUniform3f("objectColor", colour);
	lightingShader->setUniform1i("isLight", false);
	lightingShader->setUniform3f("emissionColor", glm::vec3(0.0));

	ofPushMatrix();
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

void GameObject::computeLocalBounds() {
	const std::vector<glm::vec3>& verts = mesh.getVertices();
	if (verts.empty()) {
		localMinBound = glm::vec3(0.0f);
		localMaxBound = glm::vec3(0.0f);
		return;
	}

	localMinBound = verts[0];
	localMaxBound = verts[0];

	for (const auto& v : verts) {
		localMinBound = glm::min(localMinBound, v);
		localMaxBound = glm::max(localMaxBound, v);
	}
}
