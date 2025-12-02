#include "redBloodCellParticleSystem.h"

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
        positions[i] = sphere_sample();

        normals[i] = glm::normalize(positions[i]); // normalize puts them on the surface of a sphere
        //        normals[i] = positions[i]; // plain position provides variable velocity
        //        normals[i] = positions[i]*2*length(positions[i]); // even less uniform, ones on the outside travelling even faster
        ids[i] = i;
        phases[i] = ofRandom(0, 0.25);
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
    //ofDisableDepthTest();
    glDepthMask(GL_FALSE); // dont write to depth buffer (still read)
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    //ofEnableAlphaBlending();
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
    //ofEnableAlphaBlending();
    //ofDisableAlphaBlending();
    //redBloodCell.draw(0, 0);
    //ofEnableDepthTest();
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

glm::vec3 ParticleSystem::sphere_sample() {
    glm::vec3 p;
    do {
        p = glm::vec3(
            ofRandom(-1.0f, 1.0f),
            ofRandom(-1.0f, 1.0f),
            ofRandom(-1.0f, 1.0f)
        );
    } while (glm::length2(p) > 1.0f);
    return p;
}

/*
glm::vec3 RedBloodCellParticleSystem::ring_sample() {
    float randomRadius = ofRandom((ringRadius - 0.1f) / 2, (ringRadius + 0.1f) / 2);
    float angle = ofRandom(0, TWO_PI);

    float x = (cos(angle) * randomRadius) + ofRandom(-2, 2);
    float y = 0; // ofRandom(-ringHeightVariation, ringHeightVariation);
    float z = (sin(angle) * randomRadius) + ofRandom(-2, 2);

    return glm::vec3(x, y, z);
}
*/