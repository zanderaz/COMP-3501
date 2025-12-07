#include "BloodStreamCylinder.h"


BloodStreamCylinder::BloodStreamCylinder(const ofMesh& mesh, const glm::vec3& position, float scale)
        : GameObject(mesh, position, scale) {
        node.setPosition(position);
		heightScale = 1.0f;
		widthScale = scale;
		//node.setScale(widthScale, heightScale, widthScale);
}

void BloodStreamCylinder::update(float dt) {

}

void BloodStreamCylinder::draw(ofShader* lightingShader) {
	node.setScale(widthScale, heightScale, widthScale);
	glm::mat4 world = node.getGlobalTransformMatrix();
	if (!visible) return;

	lightingShader->setUniformMatrix4f("worldMatrix", world);
	lightingShader->setUniform3f("objectColor", colour);
	lightingShader->setUniform1i("isLight", false);
	lightingShader->setUniform3f("emissionColor", glm::vec3(0.0));

    mesh.draw();
};