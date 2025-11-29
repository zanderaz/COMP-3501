#include "screenSpaceEffect.h"


void ScreenSpaceEffect::setup(int width, int height) {
    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.internalformat = GL_RGBA;
    settings.textureTarget = GL_TEXTURE_2D;
    settings.useDepth = true;
    fbo.allocate(settings);

    shader.load("shader/screen_space_quad.vert", "shader/screen_space_quad.frag");

    if (!shader.isLoaded()) {
        ofLogError() << "Post-processing shader failed to load!";
        ofExit();
    }

    /*
    // Initialize test data
    for (int i = 0; i < 40; i++) {
        cdata[i].x = ofRandom(0, width);
        cdata[i].y = ofRandom(0, height);
        cdata[i].z = ofRandom(0, 20) * ofRandom(0, 20);
        ccol[i] = glm::vec3(ofRandom(255), ofRandom(255), ofRandom(255));
    }
    */

    makeScreenQuad(width, height);
    fbo.getTexture().setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    inBloodstream = false;
}

void ScreenSpaceEffect::begin() {
    fbo.begin();
    ofClear(0, 0, 0, 255);

    /*
    if (!perspectiveTest) {
        ofSetColor(255, 100, 150);
        for (int i = 0; i < 40; i++) {
            ofSetColor(ccol[i].x, ccol[i].y, ccol[i].z);
            ofDrawCircle(cdata[i].x, cdata[i].y, cdata[i].z);
        }
    }
    */
    //cam.begin();
    //ofDrawSphere(fbo.getWidth() / 2, fbo.getHeight() / 2, -10, 100);
    //ofRotateY(45);
    //ofDrawBox(0, 0, 0, 50);
    //cam.end();
}

void ScreenSpaceEffect::end() {
    fbo.end();
}

void ScreenSpaceEffect::draw() {
    shader.begin();
    shader.setUniformTexture("tex0", fbo.getTexture(), 0);
    shader.setUniform1f("timer", ofGetElapsedTimef());
    //shader.setUniform1i("enableScanlines", enableScanlines ? 1 : 0);
    shader.setUniform1i("inBloodstream", inBloodstream ? 1 : 0);

    quad.draw();
    shader.end();
}


void ScreenSpaceEffect::makeScreenQuad(int w, int h) {
    quad.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    quad.clear();

    // flipped (because fbo has opposite coord system)
    quad.addVertex(glm::vec3(0, h, 0));
    quad.addTexCoord(glm::vec2(0.0, 0.0)); 

    quad.addVertex(glm::vec3(0, 0, 0));
    quad.addTexCoord(glm::vec2(0.0, 1.0));

    quad.addVertex(glm::vec3(w, h, 0));
    quad.addTexCoord(glm::vec2(1.0, 0.0));

    quad.addVertex(glm::vec3(w, 0, 0));
    quad.addTexCoord(glm::vec2(1.0, 1.0));
}
