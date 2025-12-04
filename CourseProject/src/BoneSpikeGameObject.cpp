#include "BoneSpikeGameObject.h"

BoneSpikeGameObject::BoneSpikeGameObject(const ofMesh& mesh, const glm::vec3& position,
    float scale, const glm::vec3& direction, float maxLength)
    : GameObject(mesh, position, scale), direction(glm::normalize(direction)), maxLength(maxLength) {

    state = SpikeState::IDLE;
    basePosition = position;

    // spike properties
    currentLength = 0.0f;
    extensionSpeed = 600.0f; // units per second
    retractionSpeed = 400.0f;
    warningDuration = 1.5f; // seconds of warning before extending
    warningTimer = 0.0f;
    damageRadius = 500.0f;

    warningColor = glm::vec3(1.0f, 0.0f, 0.0f); // red warning
    spikeColor = glm::vec3(0.8f, 0.8f, 0.9f);   // bone white

    // set initial color
    setColour(warningColor);
    setOrientationFromDirection(direction);
}

void BoneSpikeGameObject::setOrientationFromDirection(const glm::vec3& dir) {
    if (glm::length(dir) < 0.001f) {
        orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        return;
    }

    glm::vec3 defaultDir = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 axis = glm::cross(defaultDir, dir);

    if (glm::length(axis) < 0.001f) {
        if (glm::dot(defaultDir, dir) < 0.0f) {
            orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else {
            orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    else {
        float angle = acos(glm::dot(defaultDir, dir));
        orientation = glm::angleAxis(angle, glm::normalize(axis));
    }
}

void BoneSpikeGameObject::update(float deltaTime) {
    switch (state) {
    case SpikeState::WARNING:
        warningTimer += deltaTime;

        // pulsing effect during warning
        setColour(warningColor * (0.7f + (sin(warningTimer * 10.0f) * 0.5f + 0.5f) * 0.3f));

        if (warningTimer >= warningDuration) {
            startExtending();
        }
        break;

    case SpikeState::EXTENDING:
        currentLength += extensionSpeed * deltaTime;
        if (currentLength >= maxLength) {
            currentLength = maxLength;

            startRetracting();
        }

        // move spike forward
        setPosition(basePosition + direction * currentLength);
        setColour(spikeColor);
        break;

    case SpikeState::RETRACTING:
        currentLength -= retractionSpeed * deltaTime;
        if (currentLength <= 0.0f) {
            currentLength = 0.0f;
            state = SpikeState::IDLE;
            setVisible(false); // Hide when fully retracted
        }

        setPosition(basePosition + direction * currentLength);
        break;

    case SpikeState::IDLE:
        // Do nothing
        break;
    }
}

void BoneSpikeGameObject::draw(ofShader* lightingShader) {
    if (state == SpikeState::IDLE) return;
    GameObject::draw(lightingShader);
}

void BoneSpikeGameObject::startWarning() {
    state = SpikeState::WARNING;
    warningTimer = 0.0f;
    currentLength = 0.0f;
    setPosition(basePosition);
    setVisible(true);
    setColour(warningColor);
}

void BoneSpikeGameObject::startExtending() {
    state = SpikeState::EXTENDING;
    setColour(spikeColor);
}

void BoneSpikeGameObject::startRetracting() {
    state = SpikeState::RETRACTING;
}
