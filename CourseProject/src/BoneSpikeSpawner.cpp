#include "BoneSpikeSpawner.h"

BoneSpikeSpawner::BoneSpikeSpawner()
    : isSpikingActive(false), minigameComplete(false),
    waveInterval(3.0f), spikeDuration(2.0f), spikesPerWave(50), currentWall(0) {

    // room boundaries
    roomMin = glm::vec3(11055, -27.5f, -2070);
    roomMax = glm::vec3(12045, 227.5, -1080);

    wallPositions.resize(4);
    wallDirections.resize(4);

    wallNormals.push_back(glm::vec3(1, 0, 0));
    wallNormals.push_back(glm::vec3(-1, 0, 0));
    wallNormals.push_back(glm::vec3(0, 0, -1));
    wallNormals.push_back(glm::vec3(0, 0, 1));
}

void BoneSpikeSpawner::setup(PlayerGameObject* player) {
    this->player = player;

    // Clear any existing positions
    for (int i = 0; i < 4; i++) {
        wallPositions[i].clear();
        wallDirections[i].clear();
    }

    // calculate wall dimensions
    float wallWidth = roomMax.x - roomMin.x;
    float wallDepth = roomMax.z - roomMin.z;
    float wallHeight = roomMax.y - roomMin.y;

    // Number of rows and columns for spike positions
    int rows = 5;    // Vertical positions
    int columns = 10; // Horizontal positions

    // left wall
    for (int row = 0; row < rows; row++) {
        float y = roomMin.y + 50 + (row * (wallHeight - 100) / (rows - 1));

        for (int col = 0; col < columns; col++) {
            float z = roomMin.z + 100 + (col * (wallDepth - 200) / (columns - 1));

            wallPositions[0].push_back(glm::vec3(roomMin.x, y, z));
        }
    }

    // right wall
    for (int row = 0; row < rows; row++) {
        float y = roomMin.y + 50 + (row * (wallHeight - 100) / (rows - 1));

        for (int col = 0; col < columns; col++) {
            float z = roomMin.z + 100 + (col * (wallDepth - 200) / (columns - 1));

            wallPositions[1].push_back(glm::vec3(roomMax.x, y, z));
        }
    }

    // front wall
    for (int row = 0; row < rows; row++) {
        float y = roomMin.y + 50 + (row * (wallHeight - 100) / (rows - 1));

        for (int col = 0; col < columns; col++) {
            float x = roomMin.x + 100 + (col * (wallWidth - 200) / (columns - 1));

            wallPositions[2].push_back(glm::vec3(x, y, roomMax.z));
        }
    }

    // back wall
    for (int row = 0; row < rows; row++) {
        float y = roomMin.y + 50 + (row * (wallHeight - 100) / (rows - 1));

        for (int col = 0; col < columns; col++) {
            float x = roomMin.x + 100 + (col * (wallWidth - 200) / (columns - 1));

            wallPositions[3].push_back(glm::vec3(x, y, roomMin.z));
        }
    }

    // set direction
    for (int i = 0; i < 4; i++) {
        wallDirections[i].clear(); // Clear any existing directions
        for (size_t j = 0; j < wallPositions[i].size(); j++) {
            wallDirections[i].push_back(wallNormals[i]);
        }
    }

    /*
    // log the number of positions created for each wall
    for (int i = 0; i < 4; i++) {
        ofLog() << "Wall " << i << " has " << wallPositions[i].size() << " spike positions";
    }
    */
}

void BoneSpikeSpawner::startMinigame(float duration) {
    isSpikingActive = true;
    minigameComplete = false;
    minigameTimer.Start(duration);
    spikeWaveTimer.Start(waveInterval);
    currentWall = ofRandom(0,4);

    //ofLog() << "Bone spike minigame started! Duration: " << duration << " seconds";
}

void BoneSpikeSpawner::stopMinigame() {
    isSpikingActive = false;
    minigameComplete = true;
    clearSpikes();
    //ofLog() << "Bone spike minigame completed!";
}

void BoneSpikeSpawner::update(float deltaTime, PlayerGameObject* player) {
    if (!isSpikingActive) return;

    // check if minigame time is up
    if (minigameTimer.FinishedAndStop()) {
        stopMinigame();
        return;
    }

    // spawn new wave of spikes
    if (spikeWaveTimer.FinishedAndStop()) {
        spawnSpikeWave();
        spikeWaveTimer.Start(waveInterval);

        // cycle to next wall
        currentWall = getNextWallIndex();
    }

    // update all spikes
    for (int i = spikes.size() - 1; i >= 0; i--) {
        BoneSpikeGameObject* spike = spikes[i];
        spike->update(deltaTime);

        // remove fully retracted spikes
        if (spike->getState() == BoneSpikeGameObject::SpikeState::IDLE) {
            delete spike;
            spikes.erase(spikes.begin() + i);
        }
    }

    checkPlayerCollision(player);
}

void BoneSpikeSpawner::draw(ofShader* shader) {
    for (BoneSpikeGameObject* spike : spikes) {
        spike->draw(shader);
    }
}

int BoneSpikeSpawner::getNextWallIndex() {
    return ofRandom(0,4);
}

// helper to make arrow mesh
ofMesh createArrowSpikeMeshSimple(float shaftRadius = 8.0f, float shaftLength = 60.0f, float tipLength = 30.0f, float tipBaseRadius = 15.0f, int shaftSegments = 12, int tipSegments = 12) {
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);

    // cylindrical shaft
    vector<glm::vec3> baseVertices;
    vector<glm::vec3> topVertices;

    for (int i = 0; i < shaftSegments; i++) {
        float angle = (float)i / shaftSegments * TWO_PI;
        float nextAngle = (float)(i + 1) / shaftSegments * TWO_PI;

        glm::vec3 baseVert(
            shaftRadius * cos(angle),
            shaftRadius * sin(angle),
            0
        );
        glm::vec3 topVert(
            shaftRadius * cos(angle),
            shaftRadius * sin(angle),
            shaftLength
        );

        baseVertices.push_back(baseVert);
        topVertices.push_back(topVert);

        if (i > 0) {
            int idx = mesh.getNumVertices();

            mesh.addVertex(baseVertices[i - 1]);
            mesh.addVertex(topVertices[i - 1]);
            mesh.addVertex(baseVertices[i]);
            mesh.addVertex(topVertices[i]);

            mesh.addTriangle(idx, idx + 1, idx + 2);
            mesh.addTriangle(idx + 1, idx + 3, idx + 2);
        }
    }

    // connect
    int idx = mesh.getNumVertices();
    mesh.addVertex(baseVertices.back());
    mesh.addVertex(topVertices.back());
    mesh.addVertex(baseVertices[0]);
    mesh.addVertex(topVertices[0]);

    mesh.addTriangle(idx, idx + 1, idx + 2);
    mesh.addTriangle(idx + 1, idx + 3, idx + 2);

    glm::vec3 baseCenter(0, 0, 0);
    int baseCenterIdx = mesh.getNumVertices();
    mesh.addVertex(baseCenter);

    for (int i = 0; i < shaftSegments; i++) {
        int next = (i + 1) % shaftSegments;
        mesh.addTriangle(baseCenterIdx, i, next);
    }

    // conical tip
    glm::vec3 tipBaseCenter(0, 0, shaftLength);
    int tipBaseCenterIdx = mesh.getNumVertices();
    mesh.addVertex(tipBaseCenter);

    // tip point
    glm::vec3 tipPoint(0, 0, shaftLength + tipLength);
    int tipPointIdx = mesh.getNumVertices();
    mesh.addVertex(tipPoint);

    // tip base
    vector<glm::vec3> tipBaseVertices;
    for (int i = 0; i < tipSegments; i++) {
        float angle = (float)i / tipSegments * TWO_PI;
        glm::vec3 vert(
            tipBaseRadius * cos(angle),
            tipBaseRadius * sin(angle),
            shaftLength
        );
        tipBaseVertices.push_back(vert);
        mesh.addVertex(vert);
    }

    // tip sides
    for (int i = 0; i < tipSegments; i++) {
        int next = (i + 1) % tipSegments;
        int baseVertIdx = tipBaseCenterIdx + 2 + i;
        int nextBaseVertIdx = tipBaseCenterIdx + 2 + next;

        mesh.addTriangle(tipPointIdx, baseVertIdx, nextBaseVertIdx);
    }

    // create tip base (connects to shaft top)
    for (int i = 0; i < shaftSegments; i++) {
        float angle = (float)i / shaftSegments * TWO_PI;
        float nextAngle = (float)(i + 1) / shaftSegments * TWO_PI;

        // corresponding tip base vertex (find closest)
        int tipBaseIdx = (int)((float)i / shaftSegments * tipSegments) % tipSegments;
        int nextTipBaseIdx = (int)((float)(i + 1) / shaftSegments * tipSegments) % tipSegments;

        // shaft top vertex indices (already in mesh)
        int shaftTopIdx = i * 2 + 1; // Assuming pattern from earlier
        int nextShaftTopIdx = ((i + 1) % shaftSegments) * 2 + 1;

        // tip base vertex indices
        int tipVertIdx = tipBaseCenterIdx + 2 + tipBaseIdx;
        int nextTipVertIdx = tipBaseCenterIdx + 2 + nextTipBaseIdx;

        // create bevel triangles
        mesh.addTriangle(shaftTopIdx, tipVertIdx, nextTipVertIdx);
        mesh.addTriangle(shaftTopIdx, nextTipVertIdx, nextShaftTopIdx);
    }

    mesh.smoothNormals(30.0f);
    return mesh;
}

void BoneSpikeSpawner::spawnSpikeWave() {
    // save mesh so it doesn't recreate every time
    static ofMesh arrowMesh;
    static bool meshCreated = false;

    if (!meshCreated) {
        arrowMesh = createArrowSpikeMeshSimple(
            8.0f,    // shaft radius
            500.0f,   // shaft length (longer to start in wall)
            40.0f,   // tip length
            20.0f,   // tip base radius
            8,       // shaft segments
            8        // tip segments
        );
        meshCreated = true;
    }

    // get positions for current wall
    const std::vector<glm::vec3> positions = wallPositions[currentWall];
    const std::vector<glm::vec3> directions = wallDirections[currentWall];

    // randomly select spike positions for this wave
    std::vector<int> selectedIndices;
    for (int i = 0; i < spikesPerWave && i < positions.size(); i++) {
        int index;
        do {
            index = ofRandom(positions.size());
        } while (std::find(selectedIndices.begin(), selectedIndices.end(), index) != selectedIndices.end());
        selectedIndices.push_back(index);
    }

    for (int index : selectedIndices) {
        // adjust position to start inside the wall (move back 100 units)
        glm::vec3 adjustedPosition = positions[index] - (directions[index] * 500.0f);

        BoneSpikeGameObject* spike = new BoneSpikeGameObject(
            arrowMesh,
            adjustedPosition,  // start inside the wall
            1.0f,
            directions[index],
            500.0f // max length (from inside wall to fully extended)
        );

        
        spike->setWarningColor(glm::vec3(1.0f, 0.3f, 0.3f));
        spike->setSpikeColor(glm::vec3(0.95f, 0.95f, 0.9f));

        spikes.push_back(spike);

        spike->startWarning();
    }
}

glm::vec3 BoneSpikeSpawner::getRandomWallPositionAndDirection(glm::vec3& outDirection) {
    // randomly select which wall the spike comes from
    int wall = ofRandom(4); // 0: left, 1: right, 2: front, 3: back

    glm::vec3 position;

    switch (wall) {
    case 0: // left wall
        position = glm::vec3(
            roomMin.x,
            ofRandom(roomMin.y + 50, roomMax.y - 50),
            ofRandom(roomMin.z + 100, roomMax.z - 100)
        );
        outDirection = glm::vec3(1, 0, 0);
        break;

    case 1: // right wall
        position = glm::vec3(
            roomMax.x,
            ofRandom(roomMin.y + 50, roomMax.y - 50),
            ofRandom(roomMin.z + 100, roomMax.z - 100)
        );
        outDirection = glm::vec3(-1, 0, 0);
        break;

    case 2: // front wall
        position = glm::vec3(
            ofRandom(roomMin.x + 100, roomMax.x - 100),
            ofRandom(roomMin.y + 50, roomMax.y - 50),
            roomMax.z
        );
        outDirection = glm::vec3(0, 0, -1);
        break;

    case 3: // back wall
        position = glm::vec3(
            ofRandom(roomMin.x + 100, roomMax.x - 100),
            ofRandom(roomMin.y + 50, roomMax.y - 50),
            roomMin.z
        );
        outDirection = glm::vec3(0, 0, 1);
        break;
    }

    return position;
}

void BoneSpikeSpawner::checkPlayerCollision(PlayerGameObject* player) {
    if (!player) return;

    glm::vec3 playerPos = player->getPosition();
    float playerRadius = player->getRadius();

    for (BoneSpikeGameObject* spike : spikes) {
        if (spike->isDangerous()) {
            float distance = glm::distance(playerPos, spike->getPosition());
            float combinedRadius = playerRadius + spike->getDamageRadius();

            if (distance < combinedRadius) {
                player->takeDamage();
                break;
            }
        }
    }
}

void BoneSpikeSpawner::clearSpikes() {
    for (BoneSpikeGameObject* spike : spikes) {
        delete spike;
    }
    spikes.clear();
}