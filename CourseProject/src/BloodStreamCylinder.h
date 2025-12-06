#pragma once
#ifndef BLOOD_STREAM_CYLINDER_H_
#define BLOOD_STREAM_CYLINDER_H_

#include "GameObject.h"

class BloodStreamCylinder : public GameObject {
public:
    BloodStreamCylinder(const ofMesh& mesh, const glm::vec3& position, float scale);

    void update(float dt) override;
    void draw(ofShader* lightingShader) override;
    ofNode& getNode() { return node; }

    void setWidthScale(float ws) { widthScale = ws; }
    void setHeightScale(float hs) { heightScale = hs; }
    float getWidthScale() const { return widthScale; }
    float getHeightScale() const { return heightScale; }

private:
    ofNode node;
    float widthScale, heightScale;
};

#endif