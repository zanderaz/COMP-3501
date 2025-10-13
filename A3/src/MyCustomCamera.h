#pragma once
#include "ofCamera.h"

class MyCustomCamera : public ofCamera {
public:
    MyCustomCamera();

    void update(float deltaTime);

    glm::vec3 getqForward();
    glm::vec3 getqSide(); 
    glm::vec3 getqUp();


    void pitch(float a);
    void roll(float a);
    void yaw(float a);

private:
    float movementSpeed;
    float rotationSpeed;
    glm::vec3 position;
    glm::quat orientation;

    glm::vec3 BASE_UP;
    glm::vec3 BASE_SIDE;
    glm::vec3 BASE_FORWARD;
};
