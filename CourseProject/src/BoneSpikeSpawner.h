#pragma once
#ifndef BONE_SPIKE_SPAWNER_H_
#define BONE_SPIKE_SPAWNER_H_

#include "ofMain.h"
#include "BoneSpikeGameObject.h"
#include "PlayerGameObject.h"
#include "timer.h"
#include <vector>

class BoneSpikeSpawner {
public:

    BoneSpikeSpawner();

    void setup(PlayerGameObject* player);
    void update(float deltaTime, PlayerGameObject* player);
    void draw(ofShader* shader);

    void startMinigame(float duration = 45.0f);
    void stopMinigame();
    bool isActive() const { return isSpikingActive; }
    bool isComplete() const { return minigameComplete; }
    const Timer& getMinigameTimer() const { return minigameTimer; }

    void clearSpikes();

private:
    void spawnSpikeWave();
    glm::vec3 getRandomWallPositionAndDirection(glm::vec3& outDirection);
    void checkPlayerCollision(PlayerGameObject* player);
    int getNextWallIndex();

    std::vector<BoneSpikeGameObject*> spikes;

    Timer spikeWaveTimer;
    Timer minigameTimer;

    bool isSpikingActive;
    bool minigameComplete;

    float waveInterval;
    float spikeDuration;
    int spikesPerWave;

    glm::vec3 roomMin;
    glm::vec3 roomMax;

    int currentWall;
    std::vector<std::vector<glm::vec3>> wallPositions;
    std::vector<std::vector<glm::vec3>> wallDirections;
    std::vector<glm::vec3> wallNormals;

    PlayerGameObject* player;
};

#endif