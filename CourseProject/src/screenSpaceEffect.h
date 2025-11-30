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

    inline void setInBloodstream(bool bloodstream) { inBloodstream = bloodstream; }
    inline void setSpeedBoostActive(bool sba) { speedBoostActive = sba; }
    void setResolution(int width, int height);

private:

    void makeScreenQuad(int w, int h);

    ofFbo fbo;
    ofShader shader;
    ofMesh quad;

    bool inBloodstream;
    bool speedBoostActive;

};

#endif