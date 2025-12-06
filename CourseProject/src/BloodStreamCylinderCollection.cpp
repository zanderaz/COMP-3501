#include "BloodStreamCylinderCollection.h"

BloodStreamCylinderCollection::BloodStreamCylinderCollection(const ofMesh& cylMesh, int count) {
    rootNode.setPosition(0, 0, 0);  // Initialize root position

    for (int i = 0; i < count; i++) {
        float scale = 1.0f - 0.1f * i;
        if (scale < 0.1f) scale = 0.1f;

        BloodStreamCylinder* c = new BloodStreamCylinder(cylMesh, { 0, 0, 0 }, scale);
        cylinders.push_back(c);

        c->setWidthScale(scale);
        c->setHeightScale(1.f);

        if (i == 0) {
            // First cylinder is attached to the root
            c->getNode().setParent(rootNode);
            c->getNode().setPosition(0, 0, 0);  // Local position relative to root
        }
        else {
            BloodStreamCylinder* parent = cylinders[i - 1];
            c->getNode().setParent(parent->getNode(), true);

            float cylHeight = 50.0f;
            c->getNode().setPosition(0, cylHeight, 0);
        }
    }
}

void BloodStreamCylinderCollection::update(float dt) {
    float time = ofGetElapsedTimef();

    for (int i = 0; i < cylinders.size(); i++) {
        BloodStreamCylinder* c = cylinders[i];

        // phase offset so upper cylinders sway differently
        float sway = sin(time * 1.5f + i/4 * 0.4f) * glm::radians(8.0f);

        // sway around Z axis (side-to-side)
        c->getNode().setOrientation(
            glm::angleAxis(sway, glm::vec3(0, 0, 1))
        );
    }
}

void BloodStreamCylinderCollection::draw(ofShader* shader) {
    for (auto* c : cylinders)
        c->draw(shader);
}