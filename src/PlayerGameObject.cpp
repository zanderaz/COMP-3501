#include "PlayerGameObject.h"

PlayerGameObject::PlayerGameObject(const glm::vec3& position, float scale, float acceleration, float rotationSpeed, MyCustomCamera cam)
	: GameObject(position, scale), acceleration(acceleration), rotationSpeed(rotationSpeed), cam(cam) {
	velocity = glm::vec3(0);
    speed = 0.0f;
    maxSpeed = 400.0f;
    power_up_speed_mult = 1.f;
    initRadius = 25;
    radius = initRadius;
    minRadius = 15;
    maxRadius = 40;
    health = 3;
	mesh = ofMesh::sphere(initRadius);
}

void PlayerGameObject::update(float deltaTime) {
    // shrinking/growing
    sizeChangeTimer.FinishedAndStop();

    if (ofGetKeyPressed('q') && !sizeChangeTimer.IsRunning()) { // shrink
        if (radius - 5 >= minRadius) {
            changeSize(-5);
            rotationSpeed += 0.15;
            // cout << radius << endl;
            sizeChangeTimer.Start(0.25f); // delay so you cant spam it
        }
    }

    if (ofGetKeyPressed('e') && !sizeChangeTimer.IsRunning()) { // grow
        if (radius + 5 <= maxRadius) {
            changeSize(5);
            if (rotationSpeed >= 1.0f) {
                rotationSpeed -= 0.15;
            }
            // cout << radius << endl;
            sizeChangeTimer.Start(0.25f); // delay so you cant spam it
        }
    }

    scale = radius / initRadius;

    // w to accel up to max speed, s to deccel down to 0 (affected by size)
    float speedMult;

    // if current radius bigger than initial radius, multiply speed by radius/initRadius
    if (radius < initRadius) {
        speedMult = 1;
    }
    else {
        speedMult = 2 * (radius / initRadius);
    }
    if (ofGetKeyPressed('w')) {
        speed += acceleration * speedMult * deltaTime;
        if (speed > maxSpeed * power_up_speed_mult) speed = maxSpeed;
    }
    if (ofGetKeyPressed('s')) {
        speed -= acceleration * speedMult * deltaTime;
        if (speed < 0.0f) speed = 0.0f;
    }

    // update position
    position += getqForward() * speed * deltaTime;

    // rotation
    float rotationamt = rotationSpeed * deltaTime;
    if (ofGetKeyPressed('i')) pitch(rotationamt);
    if (ofGetKeyPressed('k')) pitch(-rotationamt);
    if (ofGetKeyPressed('j')) yaw(rotationamt);
    if (ofGetKeyPressed('l')) yaw(-rotationamt);
    if (ofGetKeyPressed('u')) roll(rotationamt);
    if (ofGetKeyPressed('o')) roll(-rotationamt);
    if (ofGetKeyPressed('y')) orientation = glm::quat(1, 0, 0, 0);

    // make sure to normalize quaternion
    orientation = glm::normalize(orientation);

    // apply transform
    setPosition(position);
    setOrientation(orientation);

    // camera position
    cam.setPosition(position - getqForward() * 150.0f + getqUp() * 40.0f); // third person camera follow (saved in case we need later and for testing)
    // cam.setPosition(position);
    cam.setOrientation(orientation);
}

// increase the power-up speed multiplier
void PlayerGameObject::powerUpSpeedIncrease() {
    power_up_speed_mult += 0.3f;
}

// draw method (first person so really not used right now)
void PlayerGameObject::draw() {
	ofPushMatrix();
	ofTranslate(position);
    ofSetColor(colour[0], colour[1], colour[2]);
	mesh.draw();
	ofPopMatrix();
}

// rotation methods
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

void PlayerGameObject::changeSize(float factor) {
    radius += factor;
    mesh = ofMesh::sphere(radius);
}