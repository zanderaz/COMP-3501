#include "screenSpaceEffect.h"

// constructor
void ScreenSpaceEffect::setup(int width, int height) {

    shader.load("shader/screen_space_quad.vert", "shader/screen_space_quad.frag");
    if (!shader.isLoaded()) {
        ofLogError() << "Post-processing shader failed to load!";
        ofExit();
    }

    setResolution(width, height);

    inBloodstream = false;
}

// set the resolution for the fbo and quad, needed when window is resized
void ScreenSpaceEffect::setResolution(int width, int height) {
    ofFbo::Settings settings;
    settings.width = width;
    settings.height = height;
    settings.internalformat = GL_RGBA;
    settings.textureTarget = GL_TEXTURE_2D;
    settings.useDepth = true;
    fbo.allocate(settings);

    makeScreenQuad(width, height);
    fbo.getTexture().setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void ScreenSpaceEffect::begin() {
    fbo.begin();
    ofClear(0, 0, 0, 255);
}

void ScreenSpaceEffect::end() {
    fbo.end();
}

void ScreenSpaceEffect::draw() {
    shader.begin();
    shader.setUniformTexture("tex0", fbo.getTexture(), 0);
    shader.setUniform1f("timer", ofGetElapsedTimef());
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
