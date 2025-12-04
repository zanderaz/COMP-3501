#include "ParticleSystem.h"

// constructor
ParticleSystem::ParticleSystem(MyCustomCamera& camera, int num_particles)
    : cam(camera) {
    particleCount = num_particles;
    particleSize = 10.0f;
    currentTime = 0.0f;

    startTime = ofGetElapsedTimef();

    ofDisableArbTex(); // may not do anything, here for compatibility

    particleNode.setPosition(glm::vec3(0, 0, 0));
}

// load particle shader based on shader filepaths
void ParticleSystem::loadShader(string vert, string frag, string geom) {
    shader.load(vert, frag, geom);
    if (!shader.isLoaded()) {
        ofLogError() << "Particle Shader failed to load!";
        ofExit();
    }
}

// load the particle image based on filepath
void ParticleSystem::loadImage(string filepath) {
    try {
        particle_image.load(filepath);
    }
    catch (...) {
        ofLogError() << "Particle Image could not be loaded, ensure the image exists in bin/data/images.";
        ofExit();
    }
}

void ParticleSystem::setupRbcParticles() {
    positions.resize(particleCount);
    normals.resize(particleCount);
    phases.resize(particleCount);
    ids.resize(particleCount);

    for (int i = 0; i < particleCount; i++) {
        positions[i] = sphere_sample(1.0f);

        normals[i] = glm::normalize(positions[i]);
        ids[i] = i;
        phases[i] = ofRandom(0, 0.25);
    }

    vbo.setVertexData(positions.data(), particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(1, &normals[0].x, 3, particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(2, phases.data(), 1, particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(3, ids.data(), 1, particleCount, GL_STATIC_DRAW);
} 

void ParticleSystem::setupInfectionParticles() {
    positions.resize(particleCount);
    normals.resize(particleCount);
    phases.resize(particleCount);
    ids.resize(particleCount);

    for (int i = 0; i < particleCount; i++) {
        positions[i] = sphere_sample(4.0f);

        normals[i] = positions[i] * 5 * length(positions[i]);
        ids[i] = i;
        phases[i] = ofRandom(0, 1.0);
    }

    vbo.setVertexData(positions.data(), particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(1, &normals[0].x, 3, particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(2, phases.data(), 1, particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(3, ids.data(), 1, particleCount, GL_STATIC_DRAW);
}

void ParticleSystem::setupSpawnPortalParticles() {
    positions.resize(particleCount);
    normals.resize(particleCount);
    phases.resize(particleCount);
    ids.resize(particleCount);

    for (int i = 0; i < particleCount; i++) {
        positions[i] = auraRingSample(60.0f);

        normals[i] = glm::normalize(positions[i]) * ofRandom(10.0f, 20.0f);
        ids[i] = i;
        phases[i] = ofRandom(0, 1.0);
    }

    vbo.setVertexData(positions.data(), particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(1, &normals[0].x, 3, particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(2, phases.data(), 1, particleCount, GL_STATIC_DRAW);
    vbo.setAttributeData(3, ids.data(), 1, particleCount, GL_STATIC_DRAW);
}

void ParticleSystem::update() {
    currentTime = ofGetElapsedTimef();
}

void ParticleSystem::draw() {

    ofEnableBlendMode(OF_BLENDMODE_ADD);
    glDepthMask(GL_FALSE); // dont write to depth buffer (still read)

    particleNode.setPosition(position);

    ofPushMatrix();
    shader.begin();

    shader.setUniform1f("pSize", particleSize); // particle point size
    shader.setUniform1f("t", currentTime); // time
    shader.setUniformTexture("texture0", particle_image.getTexture(), 1);

    ofMatrix4x4 modelMatrix = particleNode.getGlobalTransformMatrix();
    ofMatrix4x4 viewProjMatrix = cam.getModelViewProjectionMatrix();
    ofMatrix4x4 mvp = modelMatrix * viewProjMatrix;
    shader.setUniformMatrix4f("MVP", mvp);

    vbo.draw(GL_POINTS, 0, positions.size());
    
    shader.end();
    ofPopMatrix();

    glDepthMask(GL_TRUE); // turn back on depth writing
    ofDisableBlendMode();

}

void ParticleSystem::setPosition(const glm::vec3& position) {
    this->position = position;
}

void ParticleSystem::setParticleSize(float size) {
    particleSize = size;
}

void ParticleSystem::setTime(float time) {
    currentTime = time;
}

glm::vec3 ParticleSystem::sphere_sample(float rad) {
    glm::vec3 p;
    do {
        p = glm::vec3(
            ofRandom(-rad, rad),
            ofRandom(-rad, rad),
            ofRandom(-rad, rad)
        );
    } while (glm::length2(p) > rad);
    return p;
}

glm::vec3 ParticleSystem::auraRingSample(float rad) {
    float angle = ofRandom(0, TWO_PI); // random angle
    float x = rad * cos(angle); // using radius of the enemy sphere
    float z = rad * sin(angle);
    return glm::vec3(x, 0.0f, z);
}