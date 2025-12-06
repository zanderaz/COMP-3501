#pragma once
#ifndef BLOOD_STREAM_CYLINDER_COLLECTION_H_
#define BLOOD_STREAM_CYLINDER_COLLECTION_H_

#include "GameObject.h"
#include "BloodStreamCylinder.h"

class BloodStreamCylinderCollection {
public:

    BloodStreamCylinderCollection(const ofMesh& cylMesh, int count);
    void update(float dt);
    void draw(ofShader* shader);
    void setPos(const glm::vec3& p) { rootNode.setPosition(p); }
    glm::vec3 getPos() const { return rootNode.getPosition(); }
    std::vector<BloodStreamCylinder*>& getCylinders() { return cylinders; }

private:
    ofNode rootNode; // parent
    std::vector<BloodStreamCylinder*> cylinders;
};

#endif