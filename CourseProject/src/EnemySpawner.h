#pragma once
#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "ofMain.h"
#include "EnemyGameObject.h"
#include "PlayerGameObject.h"
#include "timer.h"

class EnemySpawner {
public:
    EnemySpawner();
    void setup(ofMesh* enemyMesh);
    void update(float deltaTime, PlayerGameObject* player);
    void draw(ofShader* shader);
    void startSpawning(float interval); // spawn every x seconds
    void stopSpawning();
    void spawnEnemy();

    std::vector<EnemyGameObject*> getEnemies() { return enemies; }
    void clearEnemies();

private:
    std::vector<EnemyGameObject*> enemies;
    std::vector<glm::vec3> spawnPoints;
    ofMesh* enemyMesh;
    Timer spawnTimer;
    bool isSpawning;
    float spawnInterval;

    // arena boundaries
    glm::vec3 arenaMin;
    glm::vec3 arenaMax;
    glm::vec3 targetPoint; // point on opposite side
};

#endif