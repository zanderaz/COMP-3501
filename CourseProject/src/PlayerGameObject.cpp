#include "PlayerGameObject.h"


/*** Constructor ***/
PlayerGameObject::PlayerGameObject(const ofMesh& mesh, const glm::vec3& position, float scale, MyCustomCamera cam)
	: GameObject(mesh, position, scale), cam(cam) {

    // initialize motion params
	velocity = glm::vec3(0);
    h_accel = 500.f;
    h_drag = 1.5f;
    h_max_speed = 250.f;
    v_accel = 300.f;
    v_drag = 0.5f;
    v_max_speed = 200.f;
    gravity = 250.f;

    // initialize other params
    radius = 5; // only needed for collision purposess now
    health = 3;
}


/*** Update the player ***/
void PlayerGameObject::update(float delta_time) {

    // define foward and side vectors, remove y component
    glm::vec3 forward = getqForward();
    glm::vec3 side = getqSide();
    forward.y = 0.0f; side.y = 0.0f;

    if (glm::length2(forward) > 0.0001) {
        forward = glm::normalize(forward);
    }
    if (glm::length2(side) > 0.0001) {
        side = glm::normalize(side);
    }

    // handle horizontal direction of acceleration
    glm::vec3 accelDir(0);
    if (ofGetKeyPressed('w') || ofGetKeyPressed('W')) {
        accelDir += forward; // foward
    }
    if (ofGetKeyPressed('a') || ofGetKeyPressed('A')) {
        accelDir -= side; // left
    }
    if (ofGetKeyPressed('s') || ofGetKeyPressed('S')) {
        accelDir -= forward; // backward
    }
    if (ofGetKeyPressed('d') || ofGetKeyPressed('D')) {
        accelDir += side; // right
    }

    // normalize if there is input, push x and z components to velocity
    if (glm::length2(accelDir) > 0.0001) {
        accelDir = glm::normalize(accelDir);
        velocity.x += accelDir.x * h_accel * delta_time;
        velocity.z += accelDir.z * h_accel * delta_time;
    }

    // handle vertical direction of acceleration 
    if (ofGetKeyPressed(' ')) {
        velocity.y += v_accel * delta_time;
    }
    else {
        velocity.y -= gravity * delta_time;
    }

    // apply air drag
    velocity.x -= velocity.x * h_drag * delta_time;
    velocity.z -= velocity.z * h_drag * delta_time;
    velocity.y -= velocity.y * v_drag * delta_time;

    // prevent going over speed limits
    glm::vec2 horizontal(velocity.x, velocity.z);
    float horizontal_len = glm::length(horizontal);
    if (horizontal_len > h_max_speed) {
        horizontal = (horizontal / horizontal_len) * h_max_speed;
        velocity.x = horizontal.x; velocity.z = horizontal.y;
    }
    velocity.y = glm::clamp(velocity.y, -v_max_speed, v_max_speed);

    // apply velocity to position
    position += velocity * delta_time;

    // keep the player upright (remove roll) every frame
    enforceUpright();
    setPosition(position);
    setOrientation(orientation);

    // camera position
    cam.setPosition(position);
    cam.setOrientation(orientation);
}


/*** Draw the player (empty for 1st person) ***/
void PlayerGameObject::draw(ofShader* lightingShader) {}


/*** Rotation methods ***/
void PlayerGameObject::pitch(float amt) {

    glm::vec3 forward = orientation * glm::vec3(0, 0, -1);

    // calculate the current pitch, determine the desired new pitch
    float currentPitch = asin(glm::clamp(forward.y, -1.0f, 1.0f));
    float newPitch = currentPitch + amt;

    // clamp the new pitch to a safe range, prevents the forward vector from ever aligning perfectly with the world up vector
    float maxPitch = glm::radians(86.0f);
    newPitch = glm::clamp(newPitch, -maxPitch, maxPitch);

    // calculate the actual allowed rotation delta, apply clamped rotation
    float allowedAmt = newPitch - currentPitch;
    glm::quat change = glm::angleAxis(allowedAmt, glm::vec3(1, 0, 0));
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


/*** Make sure the player's roll stays consistent ***/
void PlayerGameObject::enforceUpright() {
    const glm::vec3 worldUp(0, 1, 0);

    // get foward vector
    glm::vec3 f = getqForward();
    if (glm::length2(f) < 1e-8f) return;
    f = glm::normalize(f);

    // get right vector
    glm::vec3 r = glm::cross(f, worldUp);

    // handle the case of a near-vertical forward vector, with a stable fallback
    if (glm::length2(r) < 1e-8f) {
        r = glm::normalize(glm::cross(f, glm::vec3(0, 0, 1)));
        if (glm::length2(r) < 1e-8f) r = glm::vec3(1, 0, 0);
    }
    else {
        r = glm::normalize(r);
    }

    // get up vector
    glm::vec3 u = glm::normalize(glm::cross(r, f));

    // create rotation matrix and cast to quaternion (becomes our new orientation)
    glm::mat3 M(r, u, -f);
    orientation = glm::normalize(glm::quat_cast(M));

}
