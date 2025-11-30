#include "TextBox.h"

TextBox::TextBox()
    : font(nullptr), backgroundColor(0, 0, 0, 200), textColor(255, 255, 255), borderColor(255, 255, 255), borderWidth(2.0f), padding(20.0f), maxWidth(400.0f) , visible(false)
{
    bounds = ofRectangle(0, 0, maxWidth + 2 * padding, 200);
    timer = new Timer();
}

void TextBox::setup(const std::string& text, ofTrueTypeFont* font, float maxWidth) {
    this->text = text;
    this->font = font;
    this->maxWidth = maxWidth;
    wrapText();
}

void TextBox::setPosition(float x, float y) {
    bounds.x = x;
    bounds.y = y;
}

void TextBox::setSize(float width, float height) {
    bounds.width = width;
    bounds.height = height;
}

void TextBox::setText(const std::string& text) {
    this->text = text;
    wrapText();
}

void TextBox::setFont(ofTrueTypeFont* font) {
    this->font = font;
    wrapText();
}

void TextBox::setBackgroundColor(const ofColor& color) {
    backgroundColor = color;
}

void TextBox::setTextColor(const ofColor& color) {
    textColor = color;
}

void TextBox::setBorderColor(const ofColor& color) {
    borderColor = color;
}

void TextBox::setBorderWidth(float width) {
    borderWidth = width;
}

void TextBox::setPadding(float padding) {
    this->padding = padding;
}

void TextBox::setVisible(bool visible) {
    this->visible = visible;
}

void TextBox::toggle() {
    visible = !visible;
}

void TextBox::wrapText() {
    wrappedText.clear();
    if (!font || text.empty()) return;

    std::istringstream wordsStream(text);
    std::string word;
    std::string currentLine;

    while (wordsStream >> word) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        float testWidth = font->stringWidth(testLine);

        if (testWidth <= maxWidth) {
            currentLine = testLine;
        }
        else {
            if (!currentLine.empty()) {
                wrappedText.push_back(currentLine);
            }
            currentLine = word;
        }
    }

    if (!currentLine.empty()) {
        wrappedText.push_back(currentLine);
    }

    if (font && !wrappedText.empty()) {
        float lineHeight = font->getLineHeight();
        bounds.height = (wrappedText.size() * lineHeight) + (2 * padding);
    }
}

void TextBox::draw() {
    if (!visible || !font) return;

    if (timer->FinishedAndStop()) {
        visible = false;
        return;
    }

    // Draw background
    ofSetColor(backgroundColor);
    ofDrawRectangle(bounds);

    // Draw border
    if (borderWidth > 0) {
        ofSetColor(borderColor);
        ofNoFill();
        ofSetLineWidth(borderWidth);
        ofDrawRectangle(bounds);
        ofFill();
    }

    // Draw text
    ofSetColor(textColor);
    float lineHeight = font->getLineHeight();
    float y = bounds.y + padding + lineHeight;

    for (const auto& line : wrappedText) {
        float x = bounds.x + padding;
        font->drawString(line, x, y);
        y += lineHeight;
    }
}

void TextBox::showTemporarily(float seconds) {
    this->visible = true;
    timer->Start(seconds);
}

void TextBox::toggleWithTimer(float seconds) {
    this->visible = !this->visible;
    if (this->visible) {
        timer->Start(seconds);
    }
}