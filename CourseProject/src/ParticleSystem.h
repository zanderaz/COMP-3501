#pragma once

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <ofMain.h>
#include <glm/glm.hpp>
#include "MyCustomCamera.h"


class ParticleSystem {

public:

    ParticleSystem(MyCustomCamera& camera, int num_particles);
    void loadShader(string vert, string frag, string geom);
    void loadImage(string filepath);

    // different setups
    void setupRbcParticles();
    void setupInfectionParticles(void);
    void setupSpawnPortalParticles(void);

    void update();
    void draw();
    void setPosition(const glm::vec3& position);
    void setParticleSize(float size);
    void setTime(float time);

private:

    glm::vec3 sphere_sample(float rad);
    glm::vec3 auraRingSample(float rad);

    ofVbo vbo; // vertex buffer object to store all the particle data
    ofNode particleNode; // ofNode for using oF transform functions; will apply the node's transform to the particle system

    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<glm::float32> phases;
    vector<glm::float32> ids;

    ofImage particle_image;
    ofShader shader;
    MyCustomCamera& cam;

    float particleSize;
    float currentTime;
    int particleCount;
    glm::vec3 position;

    float startTime;
};

#endif