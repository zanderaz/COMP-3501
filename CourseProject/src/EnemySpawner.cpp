#include "EnemySpawner.h"

EnemySpawner::EnemySpawner() : isSpawning(false), spawnInterval(1.0f) {
    // enemy spawn points
    spawnPoints = {
        glm::vec3(-3925, 0, 1645),
        glm::vec3(-3925, 0, 1400),
        glm::vec3(-3925, 0, 980),
        glm::vec3(-3925, 0, 600),
        glm::vec3(-3925, 0, 280),
        glm::vec3(-3500, 0, 280),
        glm::vec3(-3050, 0, 280),
        glm::vec3(-3050, 0, 750),
        glm::vec3(-3050, 0, 1160),
        glm::vec3(-3050, 0, 1645),
        glm::vec3(-3500, 0, 1645)
    };

    // arena boundaries
    arenaMin = glm::vec3(-3950, 0, 255);
    arenaMax = glm::vec3(-3010, 200, 1670);

    targetPoint = glm::vec3(0, 0, 0);
}

void EnemySpawner::setup(ofMesh* mesh) {
    enemyMesh = mesh;
}

void EnemySpawner::startSpawning(float interval) {
    isSpawning = true;
    spawnInterval = interval;
    spawnTimer.Start(spawnInterval);
}

void EnemySpawner::stopSpawning() {
    isSpawning = false;
}

void EnemySpawner::spawnEnemy() {
    if (!enemyMesh || spawnPoints.empty()) return;

    // randomly select a spawn point
    int spawnIndex = ofRandom(spawnPoints.size());
    glm::vec3 spawnPos = spawnPoints[spawnIndex];
    // random y pos in arena
    spawnPos.y = ofRandom(-25, 200);

    //for testing spawn pos
    //ofLog() << "Spawning enemy at position: " << spawnPos;

    // create enemy at spawn point
    EnemyGameObject* enemy = new EnemyGameObject(*enemyMesh, spawnPos, 1.0f);

    // Define wall positions

    const float LEFT_WALL_X = arenaMin.x;
    const float RIGHT_WALL_X = arenaMax.x;
    const float BOTTOM_WALL_Z = arenaMin.z;
    const float TOP_WALL_Z = arenaMax.z;

    glm::vec3 target;

    // check which wall the enemy is spawning from
    bool isLeftWall = fabs(spawnPos.x - LEFT_WALL_X) < 50;
    bool isRightWall = fabs(spawnPos.x - RIGHT_WALL_X) < 50;
    bool isTopWall = fabs(spawnPos.z - TOP_WALL_Z) < 50;
    bool isBottomWall = fabs(spawnPos.z - BOTTOM_WALL_Z) < 50;

    // Handle corners (spawn point could be at intersection of two walls)
    if (isLeftWall && isTopWall) {
        // top-left corner -> target should be bottom-right corner
        float targetX = arenaMax.x - 100;  // right wall
        float targetZ = arenaMin.z + 100;  // bottom wall
        target = glm::vec3(targetX, spawnPos.y, targetZ);
    }
    else if (isLeftWall && isBottomWall) {
        // bottom-left corner -> target should be top-right corner
        float targetX = arenaMax.x - 100;  // right wall
        float targetZ = arenaMax.z - 100;  // top wall
        target = glm::vec3(targetX, spawnPos.y, targetZ);
    }
    else if (isRightWall && isTopWall) {
        // top-right corner -> target should be bottom-left corner
        float targetX = arenaMin.x + 100;  // left wall
        float targetZ = arenaMin.z + 100;  // bottom wall
        target = glm::vec3(targetX, spawnPos.y, targetZ);
    }
    else if (isRightWall && isBottomWall) {
        // bottom-right corner -> target should be top-left corner
        float targetX = arenaMin.x + 100;  // left wall
        float targetZ = arenaMax.z - 100;  // top wall
        target = glm::vec3(targetX, spawnPos.y, targetZ);
    }
    else if (isLeftWall) {
        // bottom left -> target should be right wall
        float targetX = arenaMax.x - 100;
        float targetZ = ofRandom(arenaMin.z + 100, arenaMax.z - 100);
        target = glm::vec3(targetX, spawnPos.y, targetZ);
    }
    else if (isRightWall) {
        // right wall -> target should be left wall
        float targetX = arenaMin.x + 100;
        float targetZ = ofRandom(arenaMin.z + 100, arenaMax.z - 100);
        target = glm::vec3(targetX, spawnPos.y, targetZ);
    }
    else if (isTopWall) {
        // top wall -> target should be bottom wall
        float targetX = ofRandom(arenaMin.x + 100, arenaMax.x - 100);
        float targetZ = arenaMin.z + 100;
        target = glm::vec3(targetX, spawnPos.y, targetZ);
    }
    else if (isBottomWall) {
        // bottom wall -> target should be top wall
        float targetX = ofRandom(arenaMin.x + 100, arenaMax.x - 100);
        float targetZ = arenaMax.z - 100;
        target = glm::vec3(targetX, spawnPos.y, targetZ);
    }
    else {
        // determine which wall is farthest and go to that one
        float minDist = FLT_MAX;
        int farthestWall = 0; // 0: left, 1: right, 2: top, 3: bottom

        float distToLeft = glm::distance(spawnPos.x, arenaMin.x);
        float distToRight = glm::distance(spawnPos.x, arenaMax.x);
        float distToTop = glm::distance(spawnPos.z, arenaMax.z);
        float distToBottom = glm::distance(spawnPos.z, arenaMin.z);

        if (distToLeft < minDist) { minDist = distToLeft; farthestWall = 1; } // go to right
        if (distToRight < minDist) { minDist = distToRight; farthestWall = 0; } // go to left
        if (distToTop < minDist) { minDist = distToTop; farthestWall = 3; } // go to bottom
        if (distToBottom < minDist) { minDist = distToBottom; farthestWall = 2; } // go to top

        switch (farthestWall) {
        case 0: // left wall
            target = glm::vec3(arenaMin.x + 100, spawnPos.y,
                ofRandom(arenaMin.z + 100, arenaMax.z - 100));
            break;
        case 1: // right wall
            target = glm::vec3(arenaMax.x - 100, spawnPos.y,
                ofRandom(arenaMin.z + 100, arenaMax.z - 100));
            break;
        case 2: // top wall
            target = glm::vec3(ofRandom(arenaMin.x + 100, arenaMax.x - 100),
                spawnPos.y, arenaMax.z - 100);
            break;
        case 3: // bottom wall
            target = glm::vec3(ofRandom(arenaMin.x + 100, arenaMax.x - 100),
                spawnPos.y, arenaMin.z + 100);
            break;
        }
    }

    // calculate direction
    glm::vec3 direction = glm::normalize(target - spawnPos);

    // enemy velocity
    float speed = 300.0f;
    enemy->setVelocity(direction * speed);

    enemies.push_back(enemy);

    // restart timer for next spawn
    spawnTimer.Start(spawnInterval);
}

void EnemySpawner::update(float deltaTime, PlayerGameObject* player) {
    // spawn new enemies on timer
    if (isSpawning && spawnTimer.FinishedAndStop()) {
        spawnEnemy();
    }

    const glm::vec3 playerPos = player->getPosition();
    const float collisionRadius = player->getRadius();
    const float collisionRadiusSq = collisionRadius * collisionRadius;

    for (int i = enemies.size() - 1; i >= 0; i--) {
        EnemyGameObject* enemy = enemies[i];
        enemy->update(deltaTime);

        // collision with player
        const glm::vec3 toPlayer = playerPos - enemy->getPosition();
        const float combinedRadius = collisionRadius + enemy->getRadius();
        const float combinedRadiusSq = combinedRadius * combinedRadius;
        if (glm::length2(toPlayer) <= combinedRadiusSq && !player->getInvincibilityTimer().IsRunning()) {
            player->getInvincibilityTimer().Start(1.0f);
            player->setHealth(player->getHealth() - 1);

            delete enemy;
            enemies.erase(enemies.begin() + i);
            continue; // skip oob check since 
        }

        // remove enemies that are out of bounds
        glm::vec3 pos = enemy->getPosition();
        if (pos.x < arenaMin.x || pos.x > arenaMax.x ||
            pos.z < arenaMin.z || pos.z > arenaMax.z) {
            delete enemy;
            enemies.erase(enemies.begin() + i);
        }
    }
}

void EnemySpawner::draw(ofShader* shader) {
    for (auto enemy : enemies) {
        enemy->draw(shader);
    }
}

void EnemySpawner::clearEnemies() {
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}