#pragma once
#ifndef BONE_SPIKE_GAME_OBJECT_H_
#define BONE_SPIKE_GAME_OBJECT_H_

#include "GameObject.h"
#include <glm/glm.hpp>
#include "ofMain.h"

class BoneSpikeGameObject : public GameObject {
public:
    enum class SpikeState {
        WARNING,
        EXTENDING,
        RETRACTING,
        IDLE
    };

    BoneSpikeGameObject(const ofMesh& mesh, const glm::vec3& position, float scale,
        const glm::vec3& direction, float maxLength);

    void update(float deltaTime) override;
    void draw(ofShader* lightingShader) override;

    inline SpikeState getState() const { return state; }
    inline bool isDangerous() const { return state == SpikeState::EXTENDING; }
    inline float getDamageRadius() const { return damageRadius; }
    inline const glm::vec3& getDirection() const { return direction; }

    void startWarning();
    void startExtending();
    void startRetracting();

    void setWarningColor(const glm::vec3& color) { warningColor = color; }
    void setSpikeColor(const glm::vec3& color) { spikeColor = color; }
    void setOrientationFromDirection(const glm::vec3& dir);

private:
    SpikeState state;
    glm::vec3 direction;
    glm::vec3 warningColor;
    glm::vec3 spikeColor;

    float maxLength;
    float currentLength;
    float extensionSpeed;
    float retractionSpeed;
    float warningDuration;
    float warningTimer;
    float damageRadius;

    glm::vec3 basePosition;
};

#endif