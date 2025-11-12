#pragma once
#include "ofCamera.h"

class MyCustomCamera : public ofCamera {
public:
    MyCustomCamera();

    void update(float deltaTime);

    glm::vec3 getqForward() const;
    glm::vec3 getqSide() const; 
    glm::vec3 getqUp() const;


    void pitch(float a);
    void roll(float a);
    void yaw(float a);

private:
    glm::vec3 position;
    glm::quat orientation;

    glm::vec3 BASE_UP;
    glm::vec3 BASE_SIDE;
    glm::vec3 BASE_FORWARD;
};
