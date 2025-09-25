#include "PlayerGameObject.h"

PlayerGameObject::PlayerGameObject(const glm::vec3& position, float scale, float movementSpeed, float rotationSpeed, MyCustomCamera cam) 
	: GameObject(position, scale), movementSpeed(movementSpeed), rotationSpeed(rotationSpeed), cam(cam) {
	velocity = glm::vec3(0);
	mesh = ofMesh::sphere(25);
}

void PlayerGameObject::update(float deltaTime) {
    glm::vec3 move(0.0f);

    if (ofGetKeyPressed('w')) move += getqForward();
    if (ofGetKeyPressed('s')) move -= getqForward();
    if (ofGetKeyPressed('a')) move -= getqSide();
    if (ofGetKeyPressed('d')) move += getqSide();
    if (ofGetKeyPressed('q')) move += getqUp();
    if (ofGetKeyPressed('e')) move -= getqUp();

    if (glm::length(move) > 0.0f) {
        move = glm::normalize(move) * movementSpeed * deltaTime;
        position += move;
    }

    float rotationamt = rotationSpeed * deltaTime;
    if (ofGetKeyPressed('i')) pitch(rotationamt);
    if (ofGetKeyPressed('k')) pitch(-rotationamt);
    if (ofGetKeyPressed('j')) yaw(rotationamt);
    if (ofGetKeyPressed('l')) yaw(-rotationamt);
    if (ofGetKeyPressed('u')) roll(rotationamt);
    if (ofGetKeyPressed('o')) roll(-rotationamt);
    if (ofGetKeyPressed('y')) orientation = glm::quat(1, 0, 0, 0);

    orientation = glm::normalize(orientation);

    // apply transform
    setPosition(position);
    setOrientation(orientation);

    // make camera follow
    cam.setPosition(position - getqForward() * 150.0f + getqUp() * 40.0f); // camera offset
    cam.setOrientation(orientation);
}

void PlayerGameObject::draw() {
	ofPushMatrix();
    ofScale(this->getScale(), this->getScale(), this->getScale());
	ofTranslate(position);
	mesh.draw();
	ofPopMatrix();
    // cout << "yo" << endl;
}

void PlayerGameObject::pitch(float amt) {
	glm::quat change = glm::angleAxis(amt, glm::vec3(1, 0, 0));
	orientation = orientation * change;

}


void PlayerGameObject::yaw(float amt) {
	glm::quat change = glm::angleAxis(amt, glm::vec3(0, 1, 0));
	orientation = orientation * change;

}


void PlayerGameObject::roll(float amt) {
	glm::quat change = glm::angleAxis(amt, glm::vec3(0, 0, 1));
	orientation = orientation * change;
}
