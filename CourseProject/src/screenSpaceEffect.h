#pragma once
#ifndef SCREEN_SPACE_EFFECT_H
#define SCREEN_SPACE_EFFECT_H

#include "ofMain.h"

class ScreenSpaceEffect {
public:

    void setup(int width, int height);
    void begin();
    void end();
    void draw();

    void setInBloodstream(bool bloodstream) { inBloodstream = bloodstream; }
    void setResolution(int width, int height);

private:

    void makeScreenQuad(int w, int h);

    ofFbo fbo;
    ofShader shader;
    ofMesh quad;
    //ofEasyCam cam;

    bool enableScanlines;
    bool inBloodstream;
    bool perspectiveTest;

    //glm::vec3 cdata[40];
    //glm::vec3 ccol[40];
};

#endif