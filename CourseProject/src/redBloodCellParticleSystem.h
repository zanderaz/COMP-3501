#pragma once
#ifndef RED_BLOOD_CELL_PARTICLE_SYSTEM_H
#define RED_BLOOD_CELL_PARTICLE_SYSTEM_H
#include <ofMain.h>
#include <glm/glm.hpp>
#include "MyCustomCamera.h"


class RedBloodCellParticleSystem {
public:
    RedBloodCellParticleSystem(MyCustomCamera& camera) : cam(camera) {}
    void setup(int numParticles);
    void update();
    void draw();
    void setPosition(const glm::vec3& position);
    void setParticleSize(float size);
    void setTime(float time);

private:
    glm::vec3 sphere_sample();
    glm::vec3 ring_sample();

    void setupParticles(int numParticles);

    ofVbo vbo; // vertex buffer object to store all the particle data
    ofNode particleNode; // ofNode for using oF transform functions; will apply the node's transform to the particle system


    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<glm::float32> phases;
    vector<glm::float32> ids;

    ofImage redBloodCell;
    ofShader shader;
    MyCustomCamera& cam;

    float particleSize;
    float currentTime;
    int particleCount;
    glm::vec3 position;

    float startTime;
};

#endif