#include "MyCustomCamera.h"
#include "ofMain.h"
#include <glm/gtc/quaternion.hpp>

MyCustomCamera::MyCustomCamera() {
    position = glm::vec3(0, 0, 0);
    orientation = glm::quat(1, 0, 0, 0);
    BASE_SIDE = glm::vec3(1, 0, 0);
    BASE_UP = glm::vec3(0, 1, 0);
    BASE_FORWARD = glm::vec3(0, 0, 1);
}

void MyCustomCamera::update(float deltaTime) {
    // nothing yet
}

void MyCustomCamera::pitch(float amt) {
    glm::quat change = glm::angleAxis(amt, glm::vec3(1,0,0));
    orientation = orientation * change; // this is where q1 * q2 comes in
}

void MyCustomCamera::yaw(float amt) {
    glm::quat change = glm::angleAxis(amt, glm::vec3(0,1,0));
    orientation = orientation * change;
}

void MyCustomCamera::roll(float amt) {
    glm::quat change = glm::angleAxis(amt, glm::vec3(0,0,1));
    orientation = orientation * change;
}

glm::vec3 MyCustomCamera::getqForward() const {
    return orientation * ((-BASE_FORWARD)); // because we look down -z axis
}


glm::vec3 MyCustomCamera::getqSide() const {
    return orientation * BASE_SIDE;
}


glm::vec3 MyCustomCamera::getqUp() const {
    return orientation * BASE_UP;
}

