#pragma once
#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "ofMain.h"

// text box for dialog and information

class TextBox {
public:
    TextBox();

    void setup(const std::string& text, ofTrueTypeFont* font, float maxWidth = 400.0f);
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setText(const std::string& text);
    void setFont(ofTrueTypeFont* font);
    void setBackgroundColor(const ofColor& color);
    void setTextColor(const ofColor& color);
    void setBorderColor(const ofColor& color);
    void setBorderWidth(float width);
    void setPadding(float padding);
    void setVisible(bool visible);
    void toggle();

    void draw();
    bool isVisible() const { return visible; }
    ofRectangle getBounds() const { return bounds; }

private:
    std::string text;
    ofTrueTypeFont* font;
    ofRectangle bounds;
    ofColor backgroundColor;
    ofColor textColor;
    ofColor borderColor;
    float borderWidth;
    float padding;
    float maxWidth;
    bool visible;

    std::vector<std::string> wrappedText;
    void wrapText();
};

#endif