#pragma once
#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "GameObject.h"

// Inherit directly from GameObject so we get position, rotation, and matrix handling for free
class LSystem : public GameObject {
public:
    // Constructor
    LSystem(const glm::vec3& position);

    // We DO NOT override draw() or getWorldMatrix() here.
    // We use the ones from GameObject to ensure consistent behavior.

private:
    // Internal helper to build the geometry
    void generateAndSetMesh();

    std::string lsysString;
    float baseAngle;
    float baseLength;
    vector<float> randomOffsets;

    // constants for controlling how the L-system string is generated
    const unsigned short int INTERATION_COUNT = 4;
};

#endif
